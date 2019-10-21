#pragma once
#include "imageProcessing/singleton.hpp"

Singleton* Singleton::instance = NULL;

Singleton::Singleton()
{
}

Singleton* Singleton::getInstance()
{
	if (instance == NULL)
	{
		instance = new Singleton();
	}

	return instance;
}

void Singleton::push(CImage image)
{
	std::lock_guard<std::mutex> lock(mutex);
	this->letters.push_back(image);
}

std::vector<CImage> Singleton::getLetters()
{
	return this->letters;
}

void Singleton::sort()
{
	std::sort(this->letters.begin(), this->letters.end(), [](const CImage& first, const CImage& second)
	{
		return strcmp(first.name, second.name) < 0;
	});
}

void Singleton::clear()
{
	this->letters.clear();
}