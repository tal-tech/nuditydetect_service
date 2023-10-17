#include "ai_model.h"
#include "base/logging.h"
#include "init_server.h"
#include <mutex>
#include <unistd.h>
#include <iostream>
#include "dete_reg.h"
#include "curl_tool.h"
#include "cls_image_nudity.hpp"

using namespace cv;
\
std::mutex g_model_lock;
facethink::ClsImageNudity* classifier = facethink::ClsImageNudity::create("/home/nudity_detect/ai_model/model/cls_image_nudity_v1.0.0.trt", "", 1);
//if (classifier == nullptr) {
//    std::cerr << "Error: failed to init SDK" << std::endl;
//}

int alg_process(const cv::Mat &img,
                const string &requestID,
                const string &traceId,
                Json::Value &result) {
        std::vector<cv::Mat> images;
        std::vector<int> levels;
        std::vector<float> scores;
        images.push_back(img);
        try
        {
            std::unique_lock<std::mutex> lock_guard{g_model_lock};
            int ret = classifier->predict(images, levels, scores);
            if (ret){
                cout << requestID << " exception: " << ret << endl;
                return -1;
            }
            //打补丁，算法裸露标签与接口定义相反
            if(levels[0] == 1){
                result["nudity"] = 0;
            }else{
                result["nudity"] = 1;
            }
            //result["nudity"]= levels[0];
	return 0;
    }
    catch (std::exception &e) {
        cout << requestID << " exception: " << e.what() << endl;
        return -1;
    }
}
