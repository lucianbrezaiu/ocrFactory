PROJECT(dllMaker.test)

CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

ENABLE_TESTING()

add_compile_options(-std=c++11)

SET(${PROJECT_NAME}_HEADERS_DIRS
	../include
    ../include/imageProcessing
)

include_directories(${${PROJECT_NAME}_HEADERS_DIRS})

# ADD FILES THAT CONTAIN TESTS HERE!!!
SET(${PROJECT_NAME}_SOURCES
    main_test.cpp
    tests/thresholding_test.cpp
)

include_directories(E:/OpenCV4/opencv4/install/include)
link_directories(E:/OpenCV4/opencv4/install/x64/vc15/lib)

ADD_EXECUTABLE(${PROJECT_NAME} 
	${${PROJECT_NAME}_SOURCES}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
    gmock
    gtest_main
	imageProcessing
	opencv_core401d.lib
	opencv_imgproc401d.lib
	opencv_highgui401d.lib
	opencv_ml401d.lib
	opencv_video401d.lib
	opencv_features2d401d.lib
	opencv_calib3d401d.lib
	opencv_objdetect401d.lib
	opencv_flann401d.lib
	opencv_imgcodecs401d.lib
)
