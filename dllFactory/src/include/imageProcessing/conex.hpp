#pragma once
#include "opencv2\opencv.hpp"
#include "tuple"
#include "queue"


class Conex
{
private:

	//functia calculeaza componentele conexe ale unei imagini
	//returneaza o matrice de etichete, unde fiecare componenta are o eticheta specifica
	static std::tuple<int**, int> connectedComponents(cv::Mat image);

public:

	//functia separa componentele conexe ale unei imagini
	//returneaza o lista de perechi, unde fiecare pereche este de tipul (componentaConexa,numarPixeliAlbi)
	static std::vector<cv::Mat> split(cv::Mat image);
};