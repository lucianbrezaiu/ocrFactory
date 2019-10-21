#include "imageProcessing/detection.hpp"
#include "imageProcessing/projection.hpp"
#include "imageProcessing/util.hpp"
#include "imageProcessing/conex.hpp"

std::vector<int> Detection::getHorizontalSeparators(cv::Mat & image)
{
	std::vector<int> projection = Projection::calculateHorizontalProjections(image);

	//calculez limita de sus si limita de jos pentru fiecare proiectie a unui rand de text
	std::vector<int> aux;
	const int k = 4;
	for (int i = k; i < projection.size() - k; i++)
	{
		if (projection[i] == 0 && projection[i + 1] != 0)
		{
			aux.push_back(i - k);
		}

		if (projection[i] == 0 && projection[i - 1] != 0)
		{
			aux.push_back(i + k);

		}
	}

	//calculez separatorul dintre doua proiectii a randurilor de text
	std::vector<int> delimiters;
	delimiters.push_back(aux.at(0));
	for (int i = 1; i < aux.size() - 1; i += 2)
	{
		//if (abs(aux.at(i + 1) - aux.at(i)) < 5)
		//{
		//	continue;
		//}
		int average = (aux.at(i) + aux.at(i + 1)) / 2;
		delimiters.push_back(average);
	}
	delimiters.push_back(aux.at(aux.size()-1));


	////daca doua separatoare sunt prea apropiate le elimin si fac media dintre ele
	//double average = 0;
	//for (int i = 0; i < delimiters.size()-1 ; i++)
	//{
	//	int dif = delimiters[i + 1] - delimiters[i];
	//	average += dif;
	//}
	//average /= delimiters.size()-1;

	//aux.clear();
	//std::vector<int> delimiters2;
	//for (int i = 0; i < delimiters.size() - 1; i++)
	//{
	//	int dif = delimiters[i + 1] - delimiters[i];
	//	if (dif < average/2)
	//	{
	//		aux.push_back((delimiters[i] + delimiters[i+1])/2);
	//		i++;
	//	}
	//	else
	//	{
	//		aux.push_back(delimiters[i]);
	//	}
	//}
	//aux.push_back(delimiters[delimiters.size()-1]);

	//return aux;

	return delimiters;
}

std::vector<cv::Mat> Detection::getRows(cv::Mat& image, std::vector<int> delimiters)
{
	std::vector<cv::Mat> Rows;

	for (int index = 0; index < delimiters.size() - 1; index++)
	{
		int start = delimiters[index];
		int stop = delimiters[index + 1];

		cv::Mat aux = cv::Mat(stop - start + 1, image.cols, image.type(), double(0));

		for (int i = start; i <= stop; i++)
			for (int j = 0; j < image.cols; j++)
			{
				aux.at<uchar>(i - start, j) = image.at<uchar>(i, j);
			}
		Rows.push_back(aux);
	}

	return Rows;
}



cv::Mat Detection::drawHorizontalSeparators(cv::Mat& image)
{
	std::vector<int> delimiters = getHorizontalSeparators(image);

	cv::Mat result = image.clone();

	for each(int row in delimiters)
	{
		for (int col = 0; col < image.cols; col++)
		{
			result.at<uchar>(row, col) = 255;
		}
	}

	return result;
}



std::vector<int> Detection::getVerticalSeparators(cv::Mat & image)
{
	std::vector<int> projection = Projection::calculateVertical(image);

	//calculez limita din stanga si din dreapta pentru fiecare proiecte a unei litere
	std::vector<int> aux;
	for (int i = 1; i < projection.size() - 1; i++)
	{
		if (projection[i] == 0 && projection[i + 1] != 0)
		{
			aux.push_back(i - 1);
		}

		if (projection[i] == 0 && projection[i - 1] != 0)
		{
			aux.push_back(i + 1);
		}
	}

	//calculez separatorul dintre doua proiectii a literelor
	std::vector<int> delimiters;
	delimiters.push_back(aux.at(0));
	for (int i = 1; i < aux.size() - 1; i += 2)
	{
		int average = (aux.at(i) + aux.at(i + 1)) / 2;
		delimiters.push_back(average);
	}
	delimiters.push_back(aux.at(aux.size() - 1));

	return delimiters;
}

/*
std::vector<cv::Mat> Detection::getLetters(cv::Mat & image)
{
	std::vector<int> delimiters = getVerticalSeparators(image);

	std::vector<cv::Mat> Letters;

	for (int index = 0; index < delimiters.size() - 1; index++)
	{
		int start = delimiters[index];
		int stop = delimiters[index + 1];

		cv::Mat img = cv::Mat(image.rows, stop - start + 1, image.type(), double(0));

		for (int i = 0; i < image.rows; i++)
			for (int j = start; j <= stop; j++)
			{
				img.at<uchar>(i, j - start) = image.at<uchar>(i, j);
			}

		Letters.push_back(img);
	}

	return Letters;
}*/

cv::Mat Detection::drawVerticalSeparators(cv::Mat & image)
{
	std::vector<int> delimiters = getVerticalSeparators(image);

	cv::Mat result = image.clone();
	
	for each(int col in delimiters)
	{
		for (int row = 0; row < image.rows; row++)
		{
			result.at<uchar>(row, col) = 255;
		}
	}
	return result;
}

std::vector<cv::Mat> Detection::getWords(cv::Mat image)
{
	cv::Mat projection = Projection::drawVertical(image);

	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10,10), cv::Point(0, 0));
	cv::morphologyEx(projection, projection, cv::MORPH_DILATE, element);
	

	std::vector<int> delimiters = Detection::getVerticalSeparators(projection);

	//Util::saveImage("../output/gdfas.png",Detection::drawVerticalSeparators(projection));

	std::vector<cv::Mat> Words = std::vector<cv::Mat>();

	for (int index = 0; index < delimiters.size() - 1; index++)
	{
		int start = delimiters[index];
		int stop = delimiters[index + 1];

		cv::Mat aux = cv::Mat(image.rows,stop - start + 1, image.type(), double(0));

		for (int i = 0; i < image.rows; i++)
			for (int j = start; j <= stop; j++)
			{
				aux.at<uchar>(i, j - start) = image.at<uchar>(i, j);
			}

		Words.push_back(aux);
	}

	return Words;
}