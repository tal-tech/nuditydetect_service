#include "microservice_demo.h"
#include "base/base64.h"
#include "base/logging.h"
#include "base/time/time.h"
#include "ai_model.h"

/**
 * 此处对HandleRequest包装一层的目的：防止当前HandleRequest不能很好的
 * 满足后续的需求；若能满足，也可以直接在service_main.cpp的Listen中直
 * 接调用HandleRequest方法
 */
void MicroserviceDemo::ProcessRequest(std::string &response) {
    HandleRequest(response);
}

TALError MicroserviceDemo::handler(Json::Value &result) {
    TALError res;
    // 校验除请求body格式及image_url、image_base64之外的参数
    // if ((res=VerifyFaceRectangles()) != SERVICE_ERROR.E_OK) {
    //     return res;
    // }


    // TODO: 业务处理及调用模型等，下面是模拟输出结果
    auto code = alg_process(cv_image_,request_id_,trace_id_,result);
    if (code){
        return SERVICE_ERROR.E_ALG_FAILED;
    };
    return SERVICE_ERROR.E_OK;
}
