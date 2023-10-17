#ifndef __FACETHINK_API_CLS_IMAGE_RIOT_HPP__
#define __FACETHINK_API_CLS_IMAGE_RIOT_HPP__
#include <opencv2/opencv.hpp>
#include <vector>

#ifdef WIN32
#ifdef DLL_EXPORTS
#define EXPORT_CLASS __declspec(dllexport)
#define EXPORT_API extern "C" __declspec(dllexport)
#define EXPORT_CLASS_API
#else
#define EXPORT_CLASS __declspec(dllimport)
#define EXPORT_API extern "C" __declspec(dllimport)
#endif
#else
#define EXPORT_CLASS
#define EXPORT_API extern "C" __attribute__((visibility("default")))
#define EXPORT_CLASS_API __attribute__((visibility("default")))
#endif

namespace facethink {

class EXPORT_CLASS ClsImageNudity {
 public:
  /**
    * @brief SDK初始化函数, 必须先于其他函数之前调用
    * @param model_path[in] 模型文件路径
    * @param config_path [in] 配置文件路径
    * @return
    * @retval 创建成功则返回实例指针，返回nullptr则表示初始化失败
    * @remarks 初始化函数需要读取模型等文件, 耗时较长
    */
  EXPORT_CLASS_API static ClsImageNudity* create(const std::string& model_path, const std::string& config_path, int max_batch = 1);

  /**
    * @brief 模型推理接口, 判断图片中是否包含暴恐信息(仅接受BGR和RGB格式)
    * @param bgr_images[in] 输入的图像数据，仅支持BGR格式
    * @param labels [out] 图片判断结果, 0:正常图片; 1: 裸露图片
    * @param scores [out] 预测类别的置信度, 范围为[0, 1]
    * @param is_rgb_format [in] 区分输入的图片是否为RGB格式
    * @return 
    * @retval 0: 模型推理成功; -1: SDK未初始化; -2: 图片为空或者不为3通道
    */
  EXPORT_CLASS_API virtual int predict(const std::vector<cv::Mat>& images, std::vector<int>& labels, std::vector<float>& scores, bool is_rgb_format = false) = 0;

  /**
    * @brief 设置图片类别预测的阈值, (默认为0.6)
    * @param threshold [in] 新的预测阈值
    * @return 
    * @retval 0: 阈值设置成功; -1: 阈值设置失败
  */
  EXPORT_CLASS_API virtual int setThreshold(float threshold) = 0;

  /**
   * @brief 析构函数，用于释放SDK资源
   */
  EXPORT_CLASS_API virtual ~ClsImageNudity();

 protected:
  explicit ClsImageNudity(void);

 private:
  ClsImageNudity(const ClsImageNudity&) = delete;
  ClsImageNudity& operator=(const ClsImageNudity&) = delete;
  ClsImageNudity(ClsImageNudity&&) = delete;
  ClsImageNudity& operator=(ClsImageNudity&&) = delete;
};

}  // namespace facethink

#endif  // __FACETHINK_API_CLS_IMAGE_RIOT_HPP__
