﻿#pragma once   //保证头文件只被编译一次
#ifndef _DETE_REG_H
#define _DETE_REG_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <unistd.h>
#include "base64.h"

using namespace std;


class dete_reg{

public:	

	void downloadImg(cv::Mat &img, std::string &url);
	//std::string deteImg(std::string &imgBase64, std::string &imgname, std::string &url);
	static string deteImg(std::string &imgBase64, std::string &imgname, std::string &url);
	static void  detectonline(const cv::Mat &img,const string &token,string & jsonStr);
	static void reg(const cv::Mat &img, const string &token, string & jsonStr);
    static void inner_reg(const cv::Mat &img, string & jsonStr,const string &traceID);

};

#endif
