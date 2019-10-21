#pragma once
#include "opencv2\opencv.hpp"
#include <vector>
class Projection
{


public:

	//functia calculeaza proiectia pe orizontala a unei imagini
	static std::vector<int> calculateHorizontalProjections(cv::Mat& image);

	//functia deseneaza proiectia pe orizontala a unei imagini
	static cv::Mat drawHorizontalProjections(cv::Mat& image);




	//functia calculeaza proiectia pe verticala a unei imagini
	static std::vector<int> calculateVertical(cv::Mat& image);

	//functia deseneaza proiectia pe verticala a unei imagini
	static cv::Mat drawVertical(cv::Mat& image);

};