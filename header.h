//
// Created by lazycal on 2017/6/11.
//

#ifndef RAY_TRACING_HEADER_H
#define RAY_TRACING_HEADER_H

#include <iostream>
#include <cassert>
#include <opencv2/opencv.hpp>
using namespace cv;
using std::cout;
using std::endl;

#define SHOW_IMG(img){\
	cvNamedWindow(#img,CV_WINDOW_KEEPRATIO | CV_WINDOW_NORMAL);\
	cv::imshow(#img, img); \
	cv::waitKey(); \
}

#define SHOW_IMG_NO_WAIT(img){\
	cvNamedWindow(#img,CV_WINDOW_KEEPRATIO | CV_WINDOW_NORMAL);\
	cv::imshow(#img, img); \
}

#define SHOW_IMG_WAIT(img, time){\
	cvNamedWindow(#img,CV_WINDOW_KEEPRATIO | CV_WINDOW_NORMAL);\
	cv::imshow(#img, img); \
	cv::waitKey(time); \
}

#endif //RAY_TRACING_HEADER_H
