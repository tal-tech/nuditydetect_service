#include "json/json.h"
#include "paas_common.h"
#include "curl_tool.h"
#include "dete_reg.h"
#include "conf_param.h"
#include "apollo_conf.h"
typedef unsigned int uint32;
using namespace std;


void dete_reg::inner_reg(const cv::Mat &img,string & jsonStr,const string &traceID) {
	jsonStr.clear();
	std::string img_base64 = "";	
	std::vector<uchar> vecImg;
	cv::imencode(".jpg", img, vecImg);
	img_base64 = base64_encode(reinterpret_cast<const unsigned char *>(vecImg.data()), vecImg.size());	
	Json::Value tempJs;
    Json::FastWriter jswriter;
	tempJs["image_base64"] = img_base64;
    std::string postbody = jswriter.write(tempJs);
    std::string host_url{ConfParam::GetValue(APOLLO_SRCHSUB_URL, "")};
	jsonStr = curl_tool::curlPost(host_url,traceID, postbody);
}

void  dete_reg::reg(const cv::Mat &img, const string &token, string & jsonStr)
{ 
	// cout<<"*********************************************first*********************************"<<endl;
	jsonStr.clear();
    Json::Value tempJs;
    Json::FastWriter jswriter;
	std::string img_base64 = "";	
	std::vector<uchar> vecImg;
	cv::imencode(".jpg", img, vecImg);
	img_base64 = base64_encode(reinterpret_cast<const unsigned char *>(vecImg.data()), vecImg.size());
    tempJs["image_base64"] = img_base64;
    std::string postbody = jswriter.write(tempJs);

	// 海蛟的 ocr_test
    std::string access_key_id = "4815040119882752";
    std::string access_key_secret = "3ee7e06ccff441b7915784db1a5c6565";
		
	std::string host_url = "http://openai.100tal.com/aiimage/ocr/automatic-box";

	std::map<std::string, std::string> url_params;
	string url = analyze::PaasCommon::generateSignature(host_url, access_key_id, access_key_secret, postbody, url_params);
	jsonStr = curl_tool::curlPost(url,"", postbody);		
	//tiny_dnn::TimeStatic(40, "curl-reg");
	
	int retryCount = 0;
	while (jsonStr.size() == 0) { // 重新发一次
		usleep(5);
		cout<<"************************************if retry if retry**************************************"<<endl;
		jsonStr = curl_tool::curlPost(url,"",postbody);		
		if (retryCount > 10) {
			break;
		}
		retryCount++;
	}
}



void dete_reg::downloadImg(cv::Mat &img, std::string &url)
{
	std::string datastr;
	if (curl_tool::curlGet(url, datastr))
	{
		std::vector<uchar> vecData;
		vecData.assign(datastr.c_str(), datastr.c_str() + datastr.size());
		// img = cv::imdecode(vecData, CV_LOAD_IMAGE_COLOR);	 
		img = cv::imdecode(vecData, -1);//png 格式
	}
	else
	{
		std::cout << "curl downloadImg() failed\r\n";
	}
}


