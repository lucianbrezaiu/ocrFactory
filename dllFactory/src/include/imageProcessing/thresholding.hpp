#pragma once
#include "opencv2\opencv.hpp"
#include "util.hpp"

class Thresholding
{

private:

	//functia calculeaza imaginea integrala pentru o poza primita ca parametru
	static long** integralImage(cv::Mat& image);

public:
	//functia aplica threshold adaptiv unei poze primite ca parametru
	static cv::Mat adaptiveThresholding(cv::Mat& input);


};