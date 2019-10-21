#pragma once
#include "imageProcessing/conex.hpp"
#include "imageProcessing/util.hpp"
#include "imageProcessing/projection.hpp"
#include<vector>
#include <math.h>

std::tuple<int**, int> Conex::connectedComponents(cv::Mat image)
{
	std::vector<std::tuple<int, int>> N8;
	//N8.push_back(std::tuple<int, int>(-1, -1));
	N8.push_back(std::tuple<int, int>(-1, 0));
	//N8.push_back(std::tuple<int, int>(-1, 1));
	N8.push_back(std::tuple<int, int>(0, -1));
	N8.push_back(std::tuple<int, int>(0, 1));
	//N8.push_back(std::tuple<int, int>(1, -1));
	N8.push_back(std::tuple<int, int>(1, 0));
	//N8.push_back(std::tuple<int, int>(1, 1));

	int** labelMatrix = new int*[image.rows];
	for (int i = 0; i < image.rows; i++)
	{
		labelMatrix[i] = new int[image.cols];
		for (int j = 0; j < image.cols; j++)
		{
			labelMatrix[i][j] = 0;
		}
	}

	int label = 0;
	for (int x = 1; x < image.cols; x++)
	{
		for (int y = 1; y < image.rows; y++)
		{
			int color = (int)image.at<uchar>(y,x);
			if (color == 255 && labelMatrix[y][x] == 0)
			{
				label++;
				std::queue<std::tuple<int, int>> queue;
				labelMatrix[y][x] = label;
				queue.push(std::tuple<int, int>(y, x));
				while (queue.size() > 0)
				{
					std::tuple<int, int> qi = queue.front();
					queue.pop();
					for each(std::tuple<int, int> neighbor in N8)
					{
						int cx = std::get<1>(qi) + std::get<1>(neighbor);
						int cy = std::get<0>(qi) + std::get<0>(neighbor);
						if (cy >=0 && cy < image.rows && cx >=0 && cx < image.cols)
						{
							if ((int)image.ptr<uchar>(cy)[cx] == 255 && labelMatrix[cy][cx] == 0)
							{
								labelMatrix[cy][cx] = label;
								queue.push(std::tuple<int, int>(cy, cx));
							}
						}
						
					}
				}
			}
		}
	}

	return std::tuple<int**, int>(labelMatrix, label);
}

std::pair<int,int> centruGreutate(cv::Mat image)
{
	double rowSum=0, colSum=0;
	int nr = 0;

	for (int y = 0; y < image.rows; y++)
	{
		uchar* row = image.ptr<uchar>(y);
		for (int x = 0; x < image.cols; x++)
		{
			if (row[x] == 255)
			{
				rowSum += y;
				colSum += x;
				nr++;
			}
		}
	}
	rowSum /= nr;
	colSum /= nr;
	return  std::pair<int, int>(rowSum, colSum);
}

std::vector<cv::Mat> Conex::split(cv::Mat image)
{
	/*cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5), cv::Point(0, 0));
	cv::morphologyEx(image, image, cv::MORPH_ERODE, element);*/

	std::vector<cv::Mat> components;
	std::vector<cv::Mat> letters;

	std::tuple<int**, int> tuple = connectedComponents(image);
	if (std::get<1>(tuple) > 1)
	{
		int** labelMatrix = std::get<0>(tuple);
		int nr = std::get<1>(tuple);

		components = std::vector<cv::Mat>(nr);

		for (int i = 0; i < nr; i++)
		{
			components[i] = cv::Mat(image.rows, image.cols, image.type(), double(0));
		}

		for (int x = 0; x < image.cols; x++)
		{
			for (int y = 0; y < image.rows; y++)
			{
				int label = labelMatrix[y][x];
				if (label != 0)
				{
					components[label - 1].ptr(y)[x] = 255;
				}
			}
		}

		std::vector<cv::Mat> possibleLetters;
		//std::cout << "aici";
		for (int i = 0; i < nr; i++)
		{

			if (Util::resizeLetter(components[i],0).rows > 4 || Util::resizeLetter(components[i],0).cols > 4)
			{
				possibleLetters.push_back(components[i]);
			}
		}
		
		if (possibleLetters.size() == 0)
		{
			return letters;
		}

		bool okay;
		for (int i = 0; i < possibleLetters.size()-1; i++)
		{
			cv::Mat a = possibleLetters[i];
			cv::Mat b = possibleLetters[i + 1];

			std::pair<int, int> greutateA = centruGreutate(a);
			std::pair<int, int> greutateB = centruGreutate(b);


			if (abs(greutateA.second - greutateB.second) < 7)
			{
				cv::Mat together = cv::Mat(a.rows, a.cols, a.type(), double(0));
				for (int y = 0; y < together.rows; y++)
				{
					uchar* row1 = a.ptr<uchar>(y);
					uchar* row2 = b.ptr<uchar>(y);
					uchar* row3 = together.ptr<uchar>(y);
					for (int x = 0; x < together.cols; x++)
					{
						if ((long)row1[x] == 255)
						{
							row3[x] = 255;
						}
						if ((long)row2[x] == 255)
						{
							row3[x] = 255;
						}
					}
				}
				letters.push_back(together);
				i++;
				if (i + 1 == possibleLetters.size() - 1)
				{
					letters.push_back(possibleLetters[i + 1]);

				}
			}
			else
			{
				letters.push_back(possibleLetters[i]);
				if (i + 1 == possibleLetters.size() - 1)
				{
					letters.push_back(possibleLetters[i+1]);

				}
			}
		}

			
		//double average = (double)sum / nr;
		//double sum = 0;
		//for (int i = 0; i < nr; i++)
		//{
		//	//components[i] = Util::resizeLetter(components[i],0);
		//	//diag[i] = components[i].cols * components[i].rows;
		//	//sum += diag[i];
		//}


		//double average = (double)sum / nr;
		//for (int i = 0; i < nr; i++)
		//{
		//	if (diag[i] >= 0.4 * average)// ma mai gandesc la conditie
		//	{
		//		letters.push_back(Util::resizeLetter(components[i],0));
		//	}
		//}

		for (int i = 0; i < image.rows; i++)
		{
			delete labelMatrix[i];
		}
		delete labelMatrix;
	}
	else
	{
		letters.push_back(Util::resizeLetter(image, 0));
	}

	//return componentsWithSizes;
	return letters;
}
