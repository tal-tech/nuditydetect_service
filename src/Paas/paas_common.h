//
// Created by yuli on 19/06/19.
//

#ifndef  ANDROID_CURL_COMMON_H
#define  ANDROID_CURL_COMMON_H

#include <map>
#include <string>

namespace analyze {

    typedef std::map<std::string, std::string> MAP_BODY_INFO;

    class PaasCommon{
    public:
        PaasCommon();

        virtual ~PaasCommon();

        std::string GetRequestId() { return request_id; }

        static std::string generateSignature(const std::string& server_api, const std::string& access_key_id , const std::string& access_key_secret
                , const std::string& jsonstr , std::map<std::string , std::string> url_parms);
//        bool SendMsgToPaasGet(const std::string& server_api,   );

    private:
        static std::string UrlEncode(const std::string& str);

        static std::string Base64_Encode(const unsigned char* Data, int DataByte);

        std::string request_id;

    };
}
#endif //GODEYE_AUDIO_TAL_ASR_H
