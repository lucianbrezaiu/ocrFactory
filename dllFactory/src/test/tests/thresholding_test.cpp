#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>

#include "imageProcessing/main.hpp"
#include "opencv2/opencv.hpp"


class processingTest : public testing::Test {
public:
	void SetUp() override {

		std::cout << "SetUp processing  test!" << std::endl;
		return ;
	}

    void TearDown() override {

        std::cout << "TearDown processing test!" << std::endl;
        return;
    }

	cv::Mat loadImage(std::string path){
		return cv::imread(path, 0);
	}

};

TEST_F(processingTest, test1) {

	cv::Mat input = loadImage("E:/LICENTA/input/copy.jpg");

	int output1 = processing::mainTest(input);
	
	int* arr = processing::imageToArray(input);
	cv::Mat input2 = processing::arrayToImage(arr, input.rows, input.cols);
	int output2 = processing::mainTest(input2);

	ASSERT_THAT(output1, output2);
}