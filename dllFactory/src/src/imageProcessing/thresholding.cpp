#include "imageProcessing/thresholding.hpp"

long** Thresholding::integralImage(cv::Mat& image)
{
	long** result = new long*[image.rows];
	
	for (int y = 0; y < image.rows; y++)
	{
		result[y] = new long[image.cols];
		long sum = 0;
		uchar* row = image.ptr<uchar>(y);
		for (int x = 0; x < image.cols; x++)
		{
			sum += row[x];
			result[y][x] = y == 0 ? 0 : result[y - 1][x] + sum;
		}
	}
	return result;
}

cv::Mat Thresholding::adaptiveThresholding(cv::Mat& input)
{
	int t = 15;
	//int s = input.cols / 10;
	int s = 50;
	//int s = 10;
	int k = s / 2 + 1;

	cv::Mat _input = Util::borderImage(input, k);
	long** integralImg = integralImage(_input);
	cv::Mat result = cv::Mat(_input.rows, _input.cols, _input.type());
	

	for (int y = k; y < _input.rows - k; y++)
	{
		uchar* row = result.ptr<uchar>(y);
		for (int x = k; x < _input.cols - k; x++)
		{
			int x1 = y - s / 2;
			int x2 = y + s / 2;
			int y1 = x - s / 2;
			int y2 = x + s / 2;

			int count = (x2 - x1) * (y2 - y1);
			int sum = integralImg[x2][y2] - integralImg[x2][y1 - 1] - integralImg[x1 - 1][y2] + integralImg[x1 - 1][y1 - 1];
			row[x] = _input.ptr<uchar>(y)[x] * count <= sum * (100 - t) / 100.0 ? 255 : 0;
		}
	}

	for (int i = 0; i < _input.rows; i++)
	{
		delete integralImg[i];
	}
	delete integralImg;

	return Util::removeBorder(result, k);
}