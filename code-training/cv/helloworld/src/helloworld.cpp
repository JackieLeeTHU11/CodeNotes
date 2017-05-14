//============================================================================
// Name        : helloworld.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <iostream>
#include <cv.h>
#include <highgui.h>

//using namespace std;
//using namespace cv;

int main() {
	std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!
	const std::string imagename = "street.jpg";
	cv::Mat img = cv::imread(imagename, CV_LOAD_IMAGE_COLOR);
	if(img.empty()){
		std::cout<<"image read failed!"<<std::endl;
	} else {
		cv::namedWindow("Display window", cv::WINDOW_NORMAL);
		cv::imshow("Display window", img);
		cv::waitKey(0);
	}

	return 0;
}
