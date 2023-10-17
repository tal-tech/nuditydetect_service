
/*************************************************************************
	> File Name: types.h
	> Created Time: 2019-12-27 23:00 beijing
 ************************************************************************/
#pragma once   //保证头文件只被编译一次


#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <functional>
#include <time.h>
using namespace std;


struct curlData
{	
	char tag; //l 下载 d 检测 r 识别
	std::string bookname="";
	std::string imgname="";
	std::string imgUrl="";	
	cv::Mat img;
	std::string dealstr="";
	std::function<void(std::string&,std::string&,cv::Mat&,std::string&)>backFunc;
	
	void callBack()
	{
	   backFunc(bookname,imgname,img, dealstr);
	}	
	
};

