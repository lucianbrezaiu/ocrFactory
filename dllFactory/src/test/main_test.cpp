#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>

int main(int argc, char* argv[]) {

	testing::InitGoogleTest(&argc, argv);

	int testResult = RUN_ALL_TESTS();

	return testResult;


}