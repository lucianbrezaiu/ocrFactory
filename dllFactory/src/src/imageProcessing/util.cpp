#include "imageProcessing/util.hpp"
#include <experimental/filesystem>


void Util::displayImage(std::string title, cv::Mat& image)
{
	cv::namedWindow(title);
	cv::imshow(title, image);
}

cv::Mat Util::loadImage(std::string path)
{
	return cv::imread(path, 0);
}

void Util::saveImage(std::string path, cv::Mat& img)
{
	//foloseste tot timpul formatul png!!
	cv::imwrite(path, img);
}

cv::Mat Util::invert(cv::Mat image)
{
	return cv::Scalar::all(255) - image;
}

double Util::F(cv::Point A, cv::Point B, cv::Point C)
{
	return (B.x - A.x) * (C.y - A.y) - (C.x - A.x) * (B.y - A.y);
}

bool Util::insideConvex(cv::Point P, std::vector<cv::Point> points)
{
	points.push_back(points[0]);

	double sign = F(points[0], points[1], P);

	for (int i = 1; i < points.size() - 1; i++)
	{
		double newSign = F(points[i], points[i + 1], P);
		if (sign * newSign < 0)
		{
			return false;
		}
	}
	return true;
}



cv::Mat Util::resizeLetter(cv::Mat& letter,int border)
{
	 std::tuple<int, int, int, int> bounds = getBounds(letter);
	 int xMin = std::get<0>(bounds);
	 int xMax = std::get<1>(bounds);
	 int yMin = std::get<2>(bounds);
	 int yMax = std::get<3>(bounds);

	 cv::Mat auxImage = cv::Mat(yMax - yMin + 1+ border, xMax - xMin + 1+ border, letter.type(), double(0));

	 for (int i = yMin; i <= yMax; i++)
	 {
		 uchar* row = letter.ptr<uchar>(i);
		 for (int j = xMin; j <= xMax; j++)
		 {
			 auxImage.at<uchar>(i - yMin + border/2, j - xMin + border/2) = row[j];
		 }
	 }

	/* cv::Mat resized;
	 cv::resize(auxImage, resized, resized.size(), 1.5,1.5, cv::INTER_CUBIC);

	 return resized;*/
	 return auxImage;
}

std::tuple<int, int, int, int> Util::getBounds(cv::Mat& letter)
{
	int xMin = INT_MAX, xMax = INT_MIN, yMin = INT_MAX,yMax = INT_MIN;
	for (int y = 0; y < letter.rows; y++)
	{
		uchar* row = letter.ptr<uchar>(y);
		for (int x = 0; x < letter.cols; x++)
		{
			if (row[x] == 255)
			{
				if (y < yMin)
				{
					yMin = y;
				}
				if (y > yMax)
				{
					yMax = y;
				}
				if (x < xMin)
				{
					xMin = x;
				}
				if (x > xMax)
				{
					xMax = x;
				}
			}
		}
	}
	return std::tuple<int, int, int, int>(xMin, xMax, yMin, yMax);
}

cv::Mat Util::borderImage(cv::Mat& image, int thickness)
{
	cv::Mat result = cv::Mat(image.rows + 2 * thickness, image.cols + 2 * thickness, image.type(), double(0));

	for (int y = 0; y < image.rows; y++)
	{
		uchar* row = result.ptr<uchar>(y + thickness);
		for (int x = 0; x < image.cols; x++)
		{
			row[x + thickness] = image.ptr<uchar>(y)[x];
		}
	}
	return result;
}

cv::Mat Util::removeBorder(cv::Mat& image, int thickness)
{
	cv::Mat result = cv::Mat(image.rows - 2 * thickness, image.cols - 2 * thickness, image.type());

	for (int y = thickness; y < image.rows - thickness; y++)
	{
		uchar* row = result.ptr<uchar>(y - thickness);
		for (int x = thickness; x < image.cols - thickness; x++)
		{
			row[x - thickness] = image.ptr<uchar>(y)[x];
		}
	}

	return result;
}