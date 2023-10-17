#pragma once
#include <curl/curl.h>
#include<string>
#include<iostream>

class curl_tool
{
  public:
  curl_tool()
  {
	//curl_global_init(CURL_GLOBAL_ALL); //多线程不安全，只需要在主线程中调用
  }
  ~curl_tool()
  {
	//curl_global_cleanup();  //多线程不安全，只需要在主线程中调用
  }
  
 static  size_t write_data(void *ptr, size_t size, size_t nmemb, std::string *buffer)
  {
	unsigned long sizes = size * nmemb;
	if (buffer == NULL) {
		return 0;
	}
	buffer->append((char *)ptr, sizes);
	return sizes;
  }

 static  bool curlGet(std::string &url,std::string &result,int timeout=10)
  {	
	int ret = CURLE_OK;	
	CURL *pCurl = curl_easy_init();

	if ( NULL == pCurl)
	{
		std::cout << "Init CURL failed..." << std::endl;
		return false;
	}

    result.clear();	
	curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, timeout);//请求超时时长
	//curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 10L);//请求超时时长
	curl_easy_setopt(pCurl, CURLOPT_CONNECTTIMEOUT, 3L); //连接超时时长 
	curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1L);//允许重定向
	curl_easy_setopt(pCurl, CURLOPT_HEADER, 0L);  //若启用，会将头文件的信息作为数据流输出
	curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_data);  //得到请求结果后的回调函数	

	curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &result);
	curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1L); //关闭中断信号响应
	//curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L); //启用时会汇报所有的信息
	// curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 0);
	//curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 1L); //去掉控制台输出
	curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str()); //需要获取的URL地址
	
	curl_slist *pList = NULL;
	//pList = curl_slist_append(pList, "Accept-Encoding:gzip, deflate, sdch");
	pList = curl_slist_append(pList, "Accept-Language:zh-CN,zh;q=0.8");
	pList = curl_slist_append(pList, "Connection:close"); //keep-alive  close
	curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, pList);

	CURLcode res = curl_easy_perform(pCurl);  //执行请求

	long res_code = 0;
	curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &res_code); //正确响应
	
	curl_slist_free_all(pList);
	curl_easy_cleanup(pCurl);

	//正确响应后，请请求转写成本地文件的文件
	if ((res == CURLE_OK) && (res_code == 200 || res_code == 201))
	{					
		return true;
	}	
	printf("#####curl get res:%d ,res_code:%d error #####  \n",res,res_code);
	return false;
  }
 
 static std::string curlPost(std::string url,std::string traceId,std::string postdBody,int timeout=3)
 {	
	std::string  result;
	int ret = CURLE_OK;
	   	 
	CURL *pCurl = curl_easy_init();
	if (NULL == pCurl)
	{
		std::cout << "Init CURL failed..." << std::endl;
		return result;
	}
	
	curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, timeout);//请求超时时长 
	//curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 2L);//请求超时时长 2s CURLOPT_TIMEOUT_MS 为毫秒
	curl_easy_setopt(pCurl, CURLOPT_CONNECTTIMEOUT, 3L); //连接超时时长 
	curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1L);//允许重定向
	curl_easy_setopt(pCurl, CURLOPT_HEADER, 0L);  //若启用，会将头文件的信息作为数据流输出
	curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_data);  //得到请求结果后的回调函数
	
	curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &result);
	curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1L); //关闭中断信号响应
	// curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L); //启用时会汇报所有的信息
	//rl_easy_setopt(pCurl, CURLOPT_VERBOSE, 0);
	// curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 1L); //去掉控制台输出
	curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str()); //需要获取的URL地址

    curl_easy_setopt(pCurl, CURLOPT_FORBID_REUSE, 1); //在完成交互以后强迫断开连接，不能重用

	curl_slist *pList = NULL;
	//pList = curl_slist_append(pList, "Accept-Encoding:gzip, deflate, sdch");
	pList = curl_slist_append(pList, "Accept-Language:zh-CN,zh;q=0.8");
	pList = curl_slist_append(pList, "Connection:close"); //keep-alive  close
	pList = curl_slist_append(pList, "Content-Type:application/json");
	pList = curl_slist_append(pList, "Expect:"); //

	if (traceId.size() > 0){
        std::string traceId_header = "Traceid:"+traceId;
	    std::cout << " traceId_header: " << traceId_header << std::endl;
	    pList = curl_slist_append(pList, traceId_header.c_str());
	}
	
	curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, pList);


     //设置post body参数
    curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, postdBody.c_str());     
    curl_easy_setopt(pCurl, CURLOPT_POST, 1); // 设置为Post
	CURLcode res =curl_easy_perform(pCurl);  //执行请求


	long res_code = 0;
	curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &res_code); //正确响应
	
	curl_slist_free_all(pList);
	curl_easy_cleanup(pCurl);

	//正确响应后，请请求转写成本地文件的文件
	if ((res == CURLE_OK) && (res_code == 200 || res_code == 201))
	{	
		return result;
	}else{
		std::cout << "request failed " << traceId << "res:" << res << "res_code" << res_code << std::endl;
	}
       
	// printf("#####curl post res:%d ,res_code:%d error #### reponse:%s  \n",res,res_code,result);
	//result.clear();
	return result;
  }
  
};
