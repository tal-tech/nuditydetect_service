#include <iostream>
#include <sstream>
#include <random>
#include <ctime>
#include "paas_common.h"
#include "HMAC_SHA1.h"

namespace analyze {

    PaasCommon::PaasCommon() {}

    PaasCommon::~PaasCommon() {}

    unsigned int random_char() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        return dis(gen);
    }

    std::string generate_hex(const unsigned int len) {
        std::stringstream ss;
        for (auto i = 0; i < len; i++) {
            const auto rc = random_char();
            std::stringstream hexstream;
            hexstream << std::hex << rc;
            auto hex = hexstream.str();
            ss << (hex.length() < 2 ? '0' + hex : hex);
        }
        return ss.str();
    }

    std::string PaasCommon::Base64_Encode(const unsigned char *Data, int DataByte) {

        const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        std::string strEncode;
        unsigned char Tmp[4] = {0};
        int LineLength = 0;
        for (int i = 0; i < (DataByte / 3); i++) {
            Tmp[1] = *Data++;
            Tmp[2] = *Data++;
            Tmp[3] = *Data++;
            strEncode += EncodeTable[Tmp[1] >> 2];
            strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
            strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
            strEncode += EncodeTable[Tmp[3] & 0x3F];
            if (LineLength += 4, LineLength == 76) {
                strEncode += "\r\n";
                LineLength = 0;
            }
        }

        int Mod = DataByte % 3;
        if (Mod == 1) {
            Tmp[1] = *Data++;
            strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
            strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
            strEncode += "==";
        } else if (Mod == 2) {
            Tmp[1] = *Data++;
            Tmp[2] = *Data++;
            strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
            strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
            strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
            strEncode += "=";
        }

        return strEncode;
    }

    std::string PaasCommon::UrlEncode(const std::string &str) {
        auto ToHex = [](unsigned char x) -> unsigned char {
            return x > 9 ? x + 55 : x + 48;
        };

        std::string strTemp = "";
        size_t length = str.length();
        for (size_t i = 0; i < length; i++) {
            if (isalnum((unsigned char) str[i]) ||
                (str[i] == '-') ||
                (str[i] == '_') ||
                (str[i] == '.') ||
                (str[i] == '~'))
                strTemp += str[i];
            else if (str[i] == ' ')
                strTemp += "+";
            else {
                strTemp += '%';
                strTemp += ToHex((unsigned char) str[i] >> 4);
                strTemp += ToHex((unsigned char) str[i] % 16);
            }
        }
        return strTemp;
    }



    std::string
    PaasCommon::generateSignature(const std::string &server_api, const std::string& access_key_id,
                                  const std::string& access_key_secret,
                                  const std::string& jsonstr ,
             std::map<std::string ,std::string> url_params) {

        std::map<std::string , std::string> signMap;
        std::string signature_nonce = generate_hex(16);
        //获坖时间
        std::time_t rawtime;
        std::tm *timeinfo;
        char buffer[80];

        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);
        std::strftime(buffer, 80, "%Y-%m-%dT%H:%M:%S", timeinfo);
        std::puts(buffer);
        std::string time_stamp = buffer;
        //生戝签坝
        std::string sha1_key = access_key_secret + "&";

        signMap.insert(std::make_pair("access_key_id" , access_key_id));
        signMap.insert(std::make_pair("timestamp",time_stamp));
        signMap.insert((std::make_pair("signature_nonce", signature_nonce)));
        std::cout<<"jsonstr size()"<<jsonstr.size()<<std::endl;
        if(!jsonstr.empty()){
            signMap.insert(std::make_pair("request_body" , jsonstr));
        }
        if (!url_params.empty()){
            for (auto itr = url_params.begin(); itr != url_params.end(); ++itr) {
                signMap.insert(std::make_pair(itr->first ,itr -> second));
            }
        }
        std::string sign_string ;
        unsigned  int size = signMap.size();
        unsigned  int count = 0;

        for(auto itr = signMap.begin() ; itr != signMap.end() ; ++itr){
            sign_string.append(itr ->first);
            sign_string.append("=");
            sign_string.append(itr ->second);
            if(count != size-1){
                sign_string.append("&");
            }
            count++;
        }

        CHMAC_SHA1 hmac_sha1(jsonstr.size());
        UINT_8 byte_sha1[20] = {'\0'};
        hmac_sha1.HMAC_SHA1((UINT_8*)sign_string.data(), sign_string.size(), (UINT_8*)sha1_key.data(), sha1_key.size(), byte_sha1);
        std::string str_base64 = Base64_Encode(byte_sha1,sizeof(byte_sha1));
        //urlcode转砝
        std::string signature = UrlEncode(str_base64);
//        signature = hex;
        std::string str_url = server_api;
        str_url.append("?access_key_id=");
        str_url.append(access_key_id);
        if(!url_params.empty()){
            for(auto itr = url_params.begin() ; itr != url_params.end() ; ++itr){
                str_url.append("&");
                str_url.append(itr -> first);
                str_url.append("=");
                str_url.append(itr->second);
            }
        }
        
        std::cout<<"apeend end"<<std::endl;
        str_url.append("&signature=");
        str_url.append(signature);
        str_url.append("&signature_nonce=");
        str_url.append(signature_nonce);
        str_url.append("&timestamp=");
        str_url.append(time_stamp);

        return str_url;
    }


}
