#pragma once
#include "opencv2\opencv.hpp"
#include "string"
#include <iostream>
#include <vector>
#include "thresholding.hpp"


class Util
{

public:

	static void displayImage(std::string title, cv::Mat& image);

	static cv::Mat loadImage(std::string path);

	static void saveImage(std::string path, cv::Mat& img);

	static cv::Mat invert(cv::Mat image);

	static double F(cv::Point A, cv::Point B, cv::Point C);

	static bool insideConvex(cv::Point P, std::vector<cv::Point> points);




	//functia elimina bordura inutila pentru o litera
	static cv::Mat resizeLetter(cv::Mat& letter,int border);

	//functia calculeaza limitele unei litere dintr-o poza
	static std::tuple<int, int, int, int> Util::getBounds(cv::Mat& letter);

	//functia adauga o margine de grosime thickness unei poze primite ca parametru
	static cv::Mat borderImage(cv::Mat& image, int thickness);

	//functia elimina o margine de grosime thickness unei poze primite ca parametru
	static cv::Mat removeBorder(cv::Mat& image, int thickness);

};