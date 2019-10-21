#include "multi-platform.hpp"

#include "opencv2/opencv.hpp"
#include "imageProcessing/CImage.hpp"
#include "imageProcessing/Package.hpp"

namespace processing
{
	void splitWord(std::vector<cv::Mat> words, int wordIndex, std::string wordName);

	void splitRow(std::vector<cv::Mat> rows, int rowIndex, std::string rowName);

	void log(std::string message, clock_t tStart);

	void execute(cv::Mat& input);

	cv::Mat resizeImage(const cv::Mat& image);

	EXPORT_C cv::Mat __stdcall arrayToImage(int* list, int rows, int cols);

	EXPORT_C int* __stdcall imageToArray(const cv::Mat_<uchar>& image);

	EXPORT_C int __stdcall mainTest(cv::Mat& input);

	EXPORT_C Package* __stdcall main(int* list, int rows, int cols);

	EXPORT_C void __stdcall myFree(Package* package);
}