#include "image_operation.h"
#include "file_download.h"
#include "base/base64.h"

#include <functional>
#include <unistd.h>
#include <vector>
#include <iostream>


using namespace base;

static bool CallWithRetry(unsigned int retry_count, 
                          unsigned int retry_sleep_millseconds, 
                          std::function<bool()> call){
    for (unsigned int i=0; i<retry_count; ++i) {
        if (call()) {
            return true;
        }
        usleep(retry_sleep_millseconds * 1000);
    }
    return false;
}

TALError GetImageData(std::string &image_data, 
                      const std::string &image_url,
                      const std::string &image_base64, 
                      unsigned int download_timeout_seconds, 
                      unsigned int retry_count, 
                      unsigned int retry_sleep_millsecods) {
    TALError error;
    if (!image_base64.empty()) {
        if (!Base64Decode(image_base64, &image_data)) {
            error = SERVICE_ERROR.E_IMAGE_BASE64_DECODE;
        } 
    } else if (!image_url.empty()) {
        auto download_image = [&]()->bool {
            double img_size = 0.0;
            std::string err_msg;
            bool res = DownloadImage(image_url, 
                                     image_data, 
                                     img_size, 
                                     err_msg, 
                                     download_timeout_seconds);
            return res;
        };
        if(!CallWithRetry(retry_count, 
                          retry_sleep_millsecods, 
                          download_image)) {
            error = SERVICE_ERROR.E_IMAGE_DOWNLOAD;
        }
    } else {
        error = SERVICE_ERROR.E_REQ_IMAGE_NULL;
    }
    return error;
}

TALError DecodeImage(cv::Mat &dest_img, 
                     const std::string &src_img) {
    std::vector<uchar> buf;
    for (const auto &item : src_img) {
        buf.push_back(item);
    }

    dest_img = cv::imdecode(buf, cv::IMREAD_COLOR);
    if ((dest_img.dims!=2) || (dest_img.rows==0) || (dest_img.cols==0)) {
        // abnormal image
        return SERVICE_ERROR.E_IMAGE_DECODE;
    }
    if ((dest_img.rows * dest_img.cols) > (4096 * 2160)) {  // 4K
        return SERVICE_ERROR.E_IMAGE_RESOLUTION;
    }
//    if ((dest_img.rows < 64) || (dest_img.cols < 64)) {
//        return SERVICE_ERROR.E_IMAGE_RESOLUTION;
//    }
//    if (((dest_img.rows/dest_img.cols) >= 20) || ((dest_img.cols/dest_img.rows) >= 20)){
//        return SERVICE_ERROR.E_IMAGE_RESOLUTION;
//    }

    return SERVICE_ERROR.E_OK;
}

ImageFormat CheckImageFormat(const std::string &image_binary) {
    if (image_binary.size() < 4) {
        return ImageFormat::UNKNOWN;
    }
    auto binary = (const unsigned char *)image_binary.c_str();
    if (binary[0]==0xFF && binary[1]==0xD8) {
        return ImageFormat::JPG;  // or return ImageFormat::JPEG;
    }
  
    if (binary[0]==0x42 && binary[1]==0x4D) {
        return ImageFormat::BMP;
    }

    if (binary[0]==0x89 && binary[1]==0x50 && 
        binary[2]==0x4E && binary[3]==0x47) {
        return ImageFormat::PNG;
    }
    return ImageFormat::UNKNOWN;
}

