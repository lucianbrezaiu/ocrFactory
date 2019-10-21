#include <iostream>
#include "opencv2/opencv.hpp"

#include "imageProcessing/thresholding.hpp"
#include "imageProcessing/projection.hpp"
#include "imageProcessing/detection.hpp"
#include "imageProcessing/conex.hpp"
#include "imageProcessing/main.hpp"
#include "imageProcessing/singleton.hpp"
#include "imageProcessing/Package.hpp"
#include  "imageProcessing/CImage.hpp"

#include "time.h"
#include "thread"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


void processing::splitWord(std::vector<cv::Mat> words, int wordIndex, std::string wordName)
{
	std::vector<cv::Mat> components = Conex::split(words[wordIndex]);

	int max = std::to_string(components.size()).length();

	Singleton* singleton = Singleton::getInstance();


	for (int i = 0; i < components.size(); i++)
	{
		cv::Mat letter = Util::resizeLetter(components[i], 0);
		letter = Util::resizeLetter(letter, 12);
		int nr = std::to_string(i).length();
		std::string colName = wordName + ".";
		for (int j = 0; j < max - nr; j++)
		{
			colName += "0";
		}

		colName += std::to_string(i);
		

		CImage* cImg = (CImage*)malloc(sizeof(CImage));
		cImg->name = new char[strlen(colName.c_str())];
		strcpy(cImg->name, colName.c_str());
		cImg->pixels = imageToArray(letter);
		cImg->rows = letter.rows;
		cImg->cols = letter.cols;

		singleton->push(*cImg);
		free(cImg);
	}
}

void processing::splitRow(std::vector<cv::Mat> rows, int rowIndex, std::string rowName)
{
		std::vector<cv::Mat> words = Detection::getWords(rows[rowIndex]);

		std::vector<std::thread> threads;

		int max = std::to_string(words.size()).length();
		for (int i = 0; i < words.size(); i++)
		{
			std::string wordName = rowName + ".";
			int nr = std::to_string(i).length();
			for (int j = 0; j < max - nr; j++)
			{
				wordName += "0";
			}
			wordName += std::to_string(i);
			threads.push_back(std::thread(splitWord, words, i, wordName));
		}

		for (int i = 0; i < words.size(); i++)
		{
			threads[i].join();
		}
		threads.clear();
}
		
void processing::log(std::string message, clock_t tStart)
{
	std::cout << message << " -> " << (double)(clock() - tStart) / CLOCKS_PER_SEC << " seconds" << std::endl;
}

void processing::execute(cv::Mat& input)
{
	clock_t tStart = clock();

	cv::Mat r1 = Thresholding::adaptiveThresholding(input);
	log("thresholding completed", tStart);
	

	cv::Mat projection = Projection::drawHorizontalProjections(r1);
	
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(1,7), cv::Point(0, 0));
	cv::morphologyEx(projection, projection, cv::MORPH_CLOSE, element);

	std::vector<int> separators = Detection::getHorizontalSeparators(projection);
	std::vector<cv::Mat> rows = Detection::getRows(r1, separators);

	log("detecting rows completed", tStart);
	
	int max = std::to_string(rows.size()).length();
	std::vector<std::thread> threads;
	for (int i = 0; i < rows.size(); i++)
	{
		cv::Mat rowToSave = rows[i];

		cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(4,4), cv::Point(0, 0));
		cv::morphologyEx(rowToSave, rowToSave, cv::MORPH_OPEN, element);

		int nr = std::to_string(i).length();
		std::string rowName = "";
		for (int j = 0; j < max - nr; j++)
		{
			rowName += "0";
		}

		rowName += std::to_string(i);
		threads.push_back(std::thread(splitRow, rows, i,rowName));
	}

	log("detecting letters completed", tStart);
	
	for (int i = 0; i < rows.size(); i++)
	{
		threads[i].join();
	}
	threads.clear();

	log("image processing completed",tStart);

}

cv::Mat processing::resizeImage(const cv::Mat& image)
{
	int desiredLength = 3500;
	int maxLength = image.cols > image.rows ? image.cols : image.rows;
	double percent = (double)(desiredLength * 100) / maxLength;
	percent /= 100;

	cv::Mat resized;
	
	cv::resize(image, resized, resized.size(), percent, percent,cv::INTER_CUBIC);
	return resized;
	
	return image;
}



cv::Mat processing::arrayToImage(int* list, int rows, int cols)
{
	cv::Mat* image = new cv::Mat(rows, cols,double(0));
	for (int y = 0; y < rows; y++)
	{
		uchar* row = image->ptr<uchar>(y);
		for (int x = 0; x < cols; x++)
		{
			row[x] = (&list[y*cols])[x];
		}
	}


	return *image;
}

int* processing::imageToArray(const cv::Mat_<uchar>& image)
{
	int* list = new int[image.rows*image.cols];

	int index = 0;
	for (int y = 0; y < image.rows; y++)
	{
		const uchar* row = image.ptr<uchar>(y);
		for (int x = 0; x < image.cols; x++)
		{
			list[index] = (int)row[x];
			index++;
		}
	}
	return list;
}


EXPORT_C Package* __stdcall processing::main(int* list, int rows, int cols)
{
	cv::Mat input = arrayToImage(list, rows, cols);

	if (input.data == NULL)
	{
		std::cout << "Opening error!";
	}

	input = resizeImage(input);

	execute(input);

	Singleton* singleton = Singleton::getInstance();
	singleton->sort();
	std::vector<CImage> letters = singleton->getLetters();
	singleton->clear();

	Package* package = (Package*)malloc(sizeof(Package) + sizeof(CImage) * letters.size());

	package->count = letters.size();
	for (int i = 0; i < letters.size(); i++)
	{
		package->letters[i].rows = letters[i].rows;
		package->letters[i].cols = letters[i].cols;
		package->letters[i].name = new char[100];
		strcpy(package->letters[i].name, letters[i].name);

		package->letters[i].pixels = letters[i].pixels;

	}

	return package;
}

EXPORT_C void __stdcall processing::myFree(Package* package)
{
	free(package);
}

EXPORT_C int __stdcall processing::mainTest(cv::Mat& input)
{
	Package* package = main(imageToArray(input),input.rows,input.cols);
	
	int count = package->count;
	myFree(package);

	return count;
}