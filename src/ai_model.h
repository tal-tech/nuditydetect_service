#pragma once

#include "json/json.h"
#include "opencv2/opencv.hpp"
#include <vector>
#include <string>

using namespace std;

int alg_process(cv::Mat const &img,
                const string &requestID, 
//                const string &keytime,
                const string &traceId,
//                const vector<string>keywords,
                Json::Value &result);