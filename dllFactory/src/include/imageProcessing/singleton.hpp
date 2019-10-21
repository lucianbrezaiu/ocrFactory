#pragma once
#include <map>
#include <opencv2/opencv.hpp>
#include "imageProcessing/CImage.hpp"

class Singleton
{
private:
	std::mutex mutex;

	std::vector<CImage> letters;

	static Singleton* instance;

	Singleton();

	

public:
	
	static Singleton* getInstance();

	void push(CImage image);

	std::vector<CImage> getLetters();
	
	void sort();

	void clear();

};