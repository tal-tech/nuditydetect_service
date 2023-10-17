#include "image_interface.h"
#include "data_flow.h"
#include "base/base64.h"
#include "base/logging.h"
#include "base/time/time.h"
#include "file_download.h"
#include "conf_param.h"
#include "apollo_conf.h"
#include "image_operation.h"


TALError ImageInterface::VerifyImageParam() {
    if (request_body_json_.isMember("image_base64")) {
        auto &image = request_body_json_["image_base64"];
        if (!image.isString()) {
            return SERVICE_ERROR.E_IMAGE_BASE64_TYPE;
        }
        image_base64_ = image.asString();
    }

    if (image_base64_.empty() && 
        request_body_json_.isMember("image_url")) {
        auto &image = request_body_json_["image_url"];
        if (!image.isString()) {
            return SERVICE_ERROR.E_IMAGE_URL_TYPE;
        }
        image_url_ = image.asString();
    }

    if (image_base64_.empty() && image_url_.empty()) {
        return SERVICE_ERROR.E_REQ_IMAGE_NULL;
    }
    return SERVICE_ERROR.E_OK;
}

TALError ImageInterface::VerifyInnerImageParam() {
    if (!request_body_json_.isMember("image_base64")) {
        return SERVICE_ERROR.E_IMAGE_BASE64_NULL;
    }
    auto &image = request_body_json_["image_base64"];
    if (!image.isString()) {
        return SERVICE_ERROR.E_IMAGE_BASE64_TYPE;
    }
    image_base64_ = image.asString();
    if (image_base64_.empty()) {
        return SERVICE_ERROR.E_IMAGE_BASE64_NULL;
    }
    return SERVICE_ERROR.E_OK;
}

TALError ImageInterface::ParseRectangleData(cv::Rect &cv_rect, 
                                            Json::Value &rectangle) {
    if (!rectangle.isMember("x") ||
        !rectangle.isMember("y") ||
        !rectangle.isMember("width") ||
        !rectangle.isMember("height")) {
        return SERVICE_ERROR.E_FACE_RECT_DATA_INCOMPLETE;
    }
    if (!rectangle["x"].isInt() ||
        !rectangle["y"].isInt() ||
        !rectangle["width"].isInt() ||
        !rectangle["height"].isInt()) {
        return SERVICE_ERROR.E_FACE_RECT_DATA_TYPE;
    }

    cv_rect.x = rectangle["x"].asInt();
    cv_rect.y = rectangle["y"].asInt();
    cv_rect.width = rectangle["width"].asInt();
    cv_rect.height = rectangle["height"].asInt();

    return SERVICE_ERROR.E_OK;
}

TALError ImageInterface::VerifyFaceRectangle(bool optional) {
    auto exist = request_body_json_.isMember("face_rectangle");
    if (!exist) {
        TALError res = SERVICE_ERROR.E_OK;
        if (!optional) {
            res= SERVICE_ERROR.E_FACE_RECT_NULL;
        }
        return res;
    }
    auto &rectangle = request_body_json_["face_rectangle"];
    if (!rectangle.isObject()) {
        return SERVICE_ERROR.E_FACE_RECT_TYPE;
    }

    cv::Rect cv_rect;
    auto res = ParseRectangleData(cv_rect, rectangle);
    if (res == SERVICE_ERROR.E_OK) {
        cv_rects_.emplace_back(cv_rect);
    }
    if (!optional && cv_rects_.empty()) {
        return SERVICE_ERROR.E_FACE_RECT_NULL;
    }
    return res;
}

TALError ImageInterface::VerifyFaceRectangles(bool optional) {
    auto exist = request_body_json_.isMember("face_rectangles");
    if (!exist) {
        TALError res = SERVICE_ERROR.E_OK;
        if (!optional) {
            res= SERVICE_ERROR.E_FACE_RECTS_NULL;
        }
        return res;
    }
    auto &rectangles = request_body_json_["face_rectangles"];
    if (!rectangles.isArray()) {
        return SERVICE_ERROR.E_FACE_RECTS_TYPE;
    }

    for (auto &rectangle : rectangles) {
        if (!rectangle.isObject()) {
            return SERVICE_ERROR.E_FACE_RECTS_ELEM_TYPE;
        }
        cv::Rect cv_rect;
        auto res = ParseRectangleData(cv_rect, rectangle);
        if (res == SERVICE_ERROR.E_OK) {
            cv_rects_.emplace_back(cv_rect);
        } else {
            return res;
        }
    }
    if (!optional && cv_rects_.empty()) {
        return SERVICE_ERROR.E_FACE_RECTS_NULL;
    }

    return SERVICE_ERROR.E_OK;
}

void ImageInterface::TransSingleURL() {
    std::string error_msg;
    auto trans_url = ConfParam::GetValue(APOLLO_DATAFLOW_URL_TRANS_HOST, "");
    auto timeout = ConfParam::GetValue(APOLLO_DATAFLOW_URL_TRANS_TIMEOUT, 1);
    auto retry = ConfParam::GetValue(APOLLO_DATAFLOW_URL_TRANS_RETRY, 1);
    std::vector<std::string> image_urls{image_url_};
    bool res = false;
    for (int i=0; i<retry; ++i) {
        res = Trans2InnerUrl(request_id_, image_urls, trans_url,
                             error_msg, timeout);
        if (res) {
            break;
        }
    }
    if (!res) {
        LOG(ERROR) << GenerateAlarmMsg(TECHNICAL_ERROR.E_URL_TRANS,
                                       interface_url_,
                                       error_msg);

    } else {
        if (image_urls.size() >= 1) {
            image_url_ = image_urls[0];
        }
    }
}

TALError ImageInterface::HandleImage() {
    TALError res;
    if (((res=ParseRequestBody())!=SERVICE_ERROR.E_OK) || 
        ((res=VerifyImageParam())!=SERVICE_ERROR.E_OK)) {
        return res;
    }
    
    if (image_base64_.empty()) {
        TransSingleURL();
    }

    std::string image_binary;
    res = GetImageData(image_binary, image_url_, image_base64_);
    if (res != SERVICE_ERROR.E_OK) {
        return res;
    }

    res = DecodeImage(cv_image_, image_binary);
    if (res != SERVICE_ERROR.E_OK) {
        return res;
    }

    if (CheckImageFormat(image_binary) == ImageFormat::UNKNOWN)
    {
        res = SERVICE_ERROR.E_IMAGE_FORMAT;
    }
    
    return res;
}

void ImageInterface::HandleRequest(std::string &response) {
    int64_t request_time = base::Time::Now().ToJavaTime();
    LOG(INFO) << "start, " << request_.raw_url;

    TALError error{SERVICE_ERROR.E_OK};
    Json::Value result;
    do {
        if ((error = HandleImage()) != SERVICE_ERROR.E_OK) {
            break;
        }
        error = handler(result);
    } while (false);

    int64_t response_time = base::Time::Now().ToJavaTime();
    double cost = response_time - request_time;

    Json::Value root;
    root["code"] = Json::Value(error.code);
    root["msg"] = Json::Value(error.message);
    auto duration = 0.0f;
    Json::Value data;
    if (error == SERVICE_ERROR.E_OK) {
        duration = cost;
        data["result"] = result;
    }
    data["process_duration"] = duration;
    root["data"] = data;
    Json::FastWriter writer;
    response = writer.write(root);
    if(strcmp(getenv("CURRENT_ENV"), "LOCAL") != 0) {
        SendDataFlow(request_time, response_time, error, response);
    }
    LOG(INFO) << "end, " << request_.raw_url
        << ", " << error << ", duration:" << cost << "ms";
    MallocTrim();
}

void ImageInterface::SendDataFlow(int64_t request_time,
                                int64_t response_time,
                                const TALError &error,
                                const std::string &response) {
    DataFlow mq_data;
    mq_data.SetValue(data_request_id, request_id_);
    if (!image_base64_.empty()) {
        mq_data.SetSourceInfos(false, image_base64_, request_id_);
    } else if (!image_url_.empty()) {
        mq_data.SetSourceInfos(true, image_url_, request_id_);
    } else {
        mq_data.SetSourceInfos(false, "", request_id_);
    }
    mq_data.SetValue(data_api_name, app_name_);
    mq_data.SetValue(data_url, interface_url_);
    mq_data.SetValue(data_appkey, app_key_);
    std::string api_id = GetURLParamValue(data_api_id);
    mq_data.SetValue(data_api_id, api_id);
    // mq_data.SetValue(data_version, app_host_);
    mq_data.SetValue(data_request_time, request_time);
    mq_data.SetValue(data_response_time, response_time);
    mq_data.SetValue(data_duration, response_time-request_time);
    int64_t res_tm = base::Time::Now().ToJavaTime();
    mq_data.SetValue(data_send_time, res_tm);
    mq_data.SetValue(data_code, error.code);
    mq_data.SetValue(data_err_code, error.code);
    mq_data.SetValue(data_msg, error.message);
    mq_data.SetValue(data_err_msg, error.message);
    mq_data.TransDataToJson(trans_response_json, response);
    mq_data.TransDataToJson(trans_body_json, request_body_);
    std::string mq_message = mq_data.GetJsonData();
    // LOG(INFO) << "data_flow: " << mq_message;
    KafkaClient::GetInstance()->SendMsg(mq_message);
}

TALError ImageInterface::HandleInnerImage() {
    TALError res;
    if (((res=ParseRequestBody())!=SERVICE_ERROR.E_OK) || 
        ((res=VerifyInnerImageParam())!=SERVICE_ERROR.E_OK)) {
        return res;
    }
    
    std::string image_binary;
    res = GetImageData(image_binary, image_url_, image_base64_);
    if (res != SERVICE_ERROR.E_OK) {
        return res;
    }

    res = DecodeImage(cv_image_, image_binary);
    if (res != SERVICE_ERROR.E_OK) {
        return res;
    }

    return res;
}

void ImageInterface::HandleInnerRequest(std::string &response) {
    int64_t request_time = base::Time::Now().ToJavaTime();
    LOG(INFO) << "start, " << request_.raw_url;

    TALError error{SERVICE_ERROR.E_OK};
    Json::Value result;
    do {
        if ((error=HandleInnerImage()) != SERVICE_ERROR.E_OK) {
            break;
        }
        error = handler(result);
    } while (false);

    int64_t response_time = base::Time::Now().ToJavaTime();
    double cost = response_time - request_time;

    Json::Value root;
    root["code"] = Json::Value(error.code);
    root["msg"] = Json::Value(error.message);
    auto duration = 0.0f;
    Json::Value data;
    if (error == SERVICE_ERROR.E_OK) {
        duration = cost;
        data["result"] = result;
    }
    data["process_duration"] = duration;
    root["data"] = data;
    Json::FastWriter writer;
    response = writer.write(root);

    LOG(INFO) << "end, " << request_.raw_url
        << ", " << error << ", duration:" << cost << "ms";
    MallocTrim();
}
