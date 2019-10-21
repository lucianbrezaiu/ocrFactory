#pragma once
#include "opencv2/opencv.hpp"
#include "vector"

class Detection
{
private:


	// functia calculeaza si returneaza coloanele care delimiteaza doua litere
	static std::vector<int> getVerticalSeparators(cv::Mat & image);

public:


	// functia calculeaza si returneaza separatorii care delimiteaza doua randuri de text
	static std::vector<int> getHorizontalSeparators(cv::Mat & image);





	// functia returneaza o lista de poze cu randuri de text
	static std::vector<cv::Mat> getRows(cv::Mat& image,std::vector<int> delimiters);

	// functia coloreaza randurile care separa doua randuri de text
	static cv::Mat drawHorizontalSeparators(cv::Mat& image);


	// functia returneaza o lista de poze cu litere
	//static std::vector<cv::Mat> getLetters(cv::Mat& image);

	// functia coloreaza coloanele care delimiteaza doua litere pentru o imagine data
	static cv::Mat drawVerticalSeparators(cv::Mat& image);


	static std::vector<cv::Mat> getWords(cv::Mat image);
};


