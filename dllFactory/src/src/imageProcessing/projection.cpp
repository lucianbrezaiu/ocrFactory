#include "imageProcessing/projection.hpp"


std::vector<int> Projection::calculateHorizontalProjections(cv::Mat& image)
{
	std::vector<int> horizontal;
	double average = 0;
	for (int y = 0; y < image.rows; y++)
	{
		int index = 0;
		uchar* row = image.ptr<uchar>(y);
		for (int x = 0; x < image.cols; x++)
		{
			if (row[x] == 255)
			{
				index++;
			}
		}
		horizontal.push_back(index);
		average += index;
	}
	average /= horizontal.size();
	average /= 2;
	for (int i = 0; i < horizontal.size(); i++)
	{
		if (horizontal.at(i) < average/2)
		{
			horizontal.at(i) = 0;
		}
	}

	return horizontal;
}

cv::Mat Projection::drawHorizontalProjections(cv::Mat& image)
{
	std::vector<int> horizontal = calculateHorizontalProjections(image);
	cv::Mat result = cv::Mat(image.rows, image.cols, image.type(), double(0));
	for (int y = 0; y < image.rows; y++)
	{
		uchar* row = result.ptr<uchar>(y);
		for (int x = 0; x < image.cols; x++)
		{
			if (x < horizontal.at(y))
			{
				row[x] = 255;
			}
		}
	}
	return result;
}

std::vector<int> Projection::calculateVertical(cv::Mat& image)
{
	std::vector<int> vertical;
	for (int x = 0; x < image.cols; x++)
	{
		int index = 0;
		for (int y = 0; y < image.rows; y++)
		{
			if (image.at<uchar>(y,x) == 255)
			{
				index++;
			}
		}
		vertical.push_back(index);
	}

	for (int index = 0; index < vertical.size(); index++)
	{
		if (vertical[index] < 5)
		{
			vertical[index] = 0;
		}
	}



	return vertical;
}

cv::Mat Projection::drawVertical(cv::Mat& image)
{
	std::vector<int> vertical = calculateVertical(image);
	cv::Mat result = cv::Mat(image.rows, image.cols, image.type(), double(0));
	
	for (int x = 0; x < image.cols; x++)
	{
		for (int y = 0; y < image.rows; y++)
		{
			if (y < vertical.at(x))
			{
				result.at<uchar>(image.rows - y - 1, x) = 255;
			}
		}
	}
	return result;
}

