PROJECT(imageProcessing)

CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

SET(${PROJECT_NAME}_HEADERS
    ../../include/multi-platform.hpp
    ../../include/imageProcessing/thresholding.hpp
	../../include/imageProcessing/util.hpp
	../../include/imageProcessing/projection.hpp
	../../include/imageProcessing/detection.hpp
	../../include/imageProcessing/conex.hpp
	../../include/imageProcessing/singleton.hpp
	../../include/imageProcessing/main.hpp
	../../include/imageProcessing/CImage.hpp
	../../include/imageProcessing/Package.hpp
)

SET(${PROJECT_NAME}_SOURCES
  conex.cpp
  detection.cpp
  main.cpp
  projection.cpp
  singleton.cpp
  thresholding.cpp
  util.cpp
)

include_directories(E:/OpenCV4/opencv4/install/include)
link_directories(E:/OpenCV4/opencv4/install/x64/vc15/lib)

ADD_LIBRARY(${PROJECT_NAME} SHARED
    ${${PROJECT_NAME}_HEADERS} 
    ${${PROJECT_NAME}_SOURCES}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
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