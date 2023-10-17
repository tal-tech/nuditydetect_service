
#include "cls_image_nudity.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <opencv2/opencv.hpp>

using namespace facethink;
int main(int argc, char* argv[]) {
  if (argc < 7) {
    std::cerr << "Error::Usage:" << argv[0] << " model_path config_path max_batch image_dir image_info_path run_loop" << std::endl;
    return -1;
  }

  const std::string model_path = argv[1];
  const std::string config_path = argv[2];
  int max_batch = std::stoi(argv[3]);
  std::string image_dir = argv[4];
  std::string image_info_path = argv[5];
  int run_loop = std::stoi(argv[6]);

  std::ofstream ofs_log("performance_testing.txt");

  ClsImageNudity* classifier = ClsImageNudity::create(model_path, config_path, max_batch);
  if (classifier == nullptr) {
    std::cerr << "Error: failed to init SDK" << std::endl;
    return -1;
  }

  for (int run_idx = 0; run_idx < run_loop || run_loop < 0; ++run_idx) {
    int total_count = 0;
    float total_cost_time = 0;
    float correct_count = 0; 
    float tp = 0;
    float fp = 0;
    float fn = 0;
    
    std::ifstream ifs_file_path(image_info_path);
    if (!ifs_file_path) {
      std::cerr << "Error: failed to open " << image_info_path << std::endl;
      break;
    }
    std::string line;
    while(std::getline(ifs_file_path, line)) {
      std::string image_name;
      std::string::size_type pos1 = line.find(" ");
      if (pos1 == std::string::npos) {
        image_name = line;
      } else {
        image_name = line.substr(0, pos1);
      }
      cv::Mat image = cv::imread(image_dir + "/" + image_name);
      if (image.empty()) {
        std::cerr << "Error:: empty image " << (image_dir + "/" + image_name) << std::endl;
        continue;
      } 
      std::string image_info = line.substr(pos1 + 1);
      std::stringstream sstr(image_info);
      int label = -1, x1 = -1, y1 = -1, width = -1, height = -1;
      sstr >> label;
      std::vector<cv::Mat> images;
      std::vector<int> levels;
      std::vector<float> scores;
      images.push_back(image);
      int ret = -1;
      auto start_time = std::chrono::steady_clock::now();
      if (label < 0 || x1 < 0 || y1 < 0 || width <= 0 || height <= 0) {
        ret = classifier->predict(images, levels, scores);
      } else {
        cv::Rect rect(x1, y1, width, height);
        ret = classifier->predict(images, levels, scores);
      }
      auto end_time = std::chrono::steady_clock::now();
      float cost_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0f;
      total_cost_time += cost_time;
      total_count += 1;
      int gt = std::stoi(image_info);
      if (levels[0] - gt == 0) {
        correct_count += 1;
      }
      if (gt == 1 && levels[0] == 1 ){
        tp += 1;
      }
      if (gt == 0 && levels[0] == 1){
        fp += 1;
      }
      if (gt == 1 && levels[0] == 0){
        fn += 1;
      }
      std::cout << image_name << '\t' << levels[0] << "\t" << label <<std::endl;
      if (ret == 0) {
        ofs_log << image_name << "," << ret << "," << cost_time << "," << levels[0] << "," << scores[0] << std::endl;
      } else {
        ofs_log << image_name << "," << ret << "," << cost_time << std::endl;
      }
    }

    if (total_count != 0) {
      float acc = correct_count / total_count;
      float recall = tp / (tp + fn);
      float precision = tp / (tp + fp);
      float f1 = 2.0 * recall * precision / (recall + precision);
      float avg_cost_time = total_cost_time / total_count;
      std::cout << "avg_cost_time:" << avg_cost_time << std::endl;
      std::cout << "acc:" << acc << std::endl;
      std::cout << "recall:" << recall << std::endl;
      std::cout << "precision:" << precision << std::endl;
      std::cout << "F1:" << f1 << std::endl;
      ofs_log << "avg_cost_time:" << avg_cost_time << std::endl;
      ofs_log << "acc:" << acc << std::endl;
      ofs_log << "recall:" << recall << std::endl;
      ofs_log << "precision:" << precision << std::endl;
      ofs_log << "F1:" << f1 << std::endl;
    }
  }

  delete classifier;
  classifier = nullptr;


  return 0;
}
