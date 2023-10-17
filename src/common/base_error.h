/**
 * 关于业务错误码的说明：
 * 业务通用错误码的取值范围：300XXX001 ~ 300XXX100
 * 图像类业务通用错误码的取值范围：300XXX101 ~ 300XXX150
 * 其他类型通用错误码保留范围：300XXX151 ~ 300XXX500
 * 具体服务接口的业务错误码范围：300XXX501 ~ 300XXX999
 *
 * 关于技术错误码的说明：
 * 技术通用错误码的取值范围：91001XXX001 ~ 91001XXX100
 * 其他类型技术通用错误码保留范围：91001XXX101 ~ 91001XXX500
 * 具体服务接口的技术错误码范围：91001XXX501 ~ 91001XXX999
 */

#pragma once

#include <string>
#include <sstream>


struct TALError {
    long code{20000};
    std::string message{"ok"};

    explicit TALError() = default;

    explicit TALError(long _code, const std::string &_message) 
        : code{_code}, message{_message} {}

    TALError(const TALError &) = default;
    TALError &operator=(const TALError &) = default;
    TALError(TALError &&) = default;
    TALError &operator=(TALError &&) = default;

    bool operator==(const TALError &rhs) {
        return code == rhs.code;
    }

    bool operator!=(const TALError &rhs) {
        return code != rhs.code;
    }

    const std::string ToString() const {
        std::string res;
        res = "error_code: " + std::to_string(code);
        res += ", error_msg: " + message; 
        return res;
    }

    std::string ToString() {
        std::string res;
        res = "error_code: " + std::to_string(code);
        res += ", error_msg: " + message; 
        return res;
    }

    friend std::ostream &operator<<(std::ostream &os, 
                                    const TALError &error) {
        os << error.ToString();
        return os;
    }
};

/**
 * 通用错误码定义:
 * 20000 ok
 * 300XXX001    "unknown input param error"
 * 300XXX002    "the request body is an illegal json"
 * 300XXX003    "the request body is not a json object"
 * ...
 * 300XXX100    # 通用错误码最大值，每个具体服务的起始值应大于此值
 */
struct CommonError {
    static const long base_code;
    static const long base_service_code;

    long sub_code;

    CommonError() = delete;
    CommonError(const long _sub_code) : sub_code{_sub_code} {}
    virtual ~CommonError() {}

    long service_code = base_code + sub_code*base_service_code;

    // 通用错误码定义
    TALError E_OK{20000, "ok"};
    TALError E_UNKNOWN_REQ{service_code+101, "unknown input param error"};
    TALError E_REQ_BODY_JSON{service_code+102,
        "the request body is an illegal json"};
    TALError E_REQ_BODY_OBJECT{service_code+103,
        "the request body is not a json object"};
};

/**
 * 图像类服务通用错误码定义：
 * 300XXX101:   "image_url type error"
 * 300XXX102:   "image download failed"
 * 300XXX103:   "image_base64 type error"
 * 300XXX104:   "image_base64 decode failed"
 * 300XXX105:   "image_url and image_base64 cannot both be null"
 * 300XXX106:   "image decode failed"
 * 300XXX107:   "unsupported image resolution"
 * 300XXX108:   "unsupported image format"
 * ......
 * 300XXX150:   # 图像类通用错误码最大值
 */
struct ImageCommonError : public CommonError {
    ImageCommonError() = delete;
    ImageCommonError(const long _sub_code) : CommonError{_sub_code} {}

    // 图像类通用错误码定义
    TALError E_IMAGE_URL_TYPE{service_code+001, "error input param"};
    TALError E_IMAGE_DOWNLOAD{service_code+005, "download image file failed"};
    TALError E_IMAGE_BASE64_TYPE{service_code+001, "error input param"};
    TALError E_IMAGE_BASE64_DECODE{service_code+006,
        "image_base64 decode failed"};
    TALError E_REQ_IMAGE_NULL{service_code+002,
        "no image data"};
    TALError E_IMAGE_DECODE{service_code+8, "failed to decode image from string"};
    TALError E_IMAGE_RESOLUTION{service_code+11,
        "picture resolution too large"};
    TALError E_IMAGE_FORMAT{service_code+7, "unsupported image format"};
    TALError E_FACE_RECTS_NULL{service_code+109, 
        "face_rectangles cannot be null"};
    TALError E_FACE_RECTS_TYPE{service_code+110, 
        "face_rectangles type error"};
    TALError E_FACE_RECTS_ELEM_TYPE{service_code+111, 
        "face_rectangles element type error"};
    TALError E_FACE_RECT_TYPE{service_code+112, "face rectangle type error"};
    TALError E_FACE_RECT_DATA_TYPE{service_code+113, 
        "face rectangle data type error"};
    TALError E_FACE_RECT_DATA_INCOMPLETE{service_code+114, 
        "face rectangle data is incomplete"};
    TALError E_FACE_RECT_NULL{service_code+115, 
        "face rectangle cannnot be null"};
    TALError E_IMAGE_BASE64_NULL{service_code+116, 
        "image_base64 cannot be null"};
};

/**
 * 技术通用错误码定义：
 * 91001XXX001:     "data flow failed"
 * 91001XXX002:     "url translation failed"
 * 91001XXX100:     # 技术通用错误码最大值
 */
struct CommonTechError : public CommonError {
    static const long tech_base_code;
    long tech_code_start =  tech_base_code + sub_code*base_service_code;

    CommonTechError() = delete;
    CommonTechError(const long _sub_code) : CommonError{_sub_code} {}

    // 通用技术错误码定义
    TALError E_DATA_FLOW{tech_code_start+1, "data flow failed"};
    TALError E_URL_TRANS{tech_code_start+2, "url translation failed"};
};


inline std::string GenerateAlarmMsg(const TALError &error, 
                                    const std::string &url, 
                                    const std::string &extra_msg="") {
    std::stringstream alarm_msg;
    alarm_msg << " alertcode:" << std::to_string(error.code);
    alarm_msg << ", alerturl:" << url;
    alarm_msg << ", alertmsg:" << error.message;
    if (!extra_msg.empty()) {
        alarm_msg << ", " << extra_msg;
    }
    return alarm_msg.str();
}

