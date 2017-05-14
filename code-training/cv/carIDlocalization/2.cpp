#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <string.h>
#include <opencv/cv.h>
#include <stdio.h>
#include "lib/normal.h"
#include <iostream>

#define DEBUG

#ifdef DEBUG
#define DE(format, ...) printf(format, ## __VA_ARGS__)
#else
#define DE(format, ...) while(0)
#endif

int main(int argc,char *argv[]){
	int threshold = 0;
	Mat img = cv::imread("car4.jpg");
	double x_beta;
	int width = img.rows;
	int height = img.cols;
	int** selection_1, selection_Number_1;
	int** address_1, address_Number_1;
	int i, j, color_num, box_flag;
	Mat img_3, img_4, img_5;
	Mat img_2;
	char str[2];
	Point s1, s2;
	Scalar color = Scalar( 0, 255, 255);

	pic_gray(img, img_2);

	threshold = histogram_Calculate(img_2, 5);
	DE("threshold:%d\n",threshold);

	pic_Thresholding(img_2, threshold);

	selection_1 = selection_Function_1(img_2, &selection_Number_1);
	for(i=0; i< selection_Number_1; i++){
		DE("selection_1[%d]:%d, %d\n", i, selection_1[i][0], selection_1[i][1]);	
	}
	
	for(i=0; i<selection_Number_1; i++){
		pic_cutting(img, img_3, selection_1, i);

		color_num = choice_Color(img_3, 110, 120);   /*蓝色为H:110--120*/
		DE("color_num:%d\n", color_num);
		int test = 0;
		if(color_num > 5){
			test += 1;
			std::cout<<"test:"<<test<<std::endl;
			IplImage pI_1 = img_3;
			cvCvtColor(&pI_1, &pI_1, CV_HSV2BGR);

			pic_gray(img_3, img_3);
			threshold = histogram_Calculate(img_3, 3);
			pic_Thresholding(img_3, threshold);

			address_1 = car_License_box(img_3, img_4, &address_Number_1);
			for(j=0; j< address_Number_1; j++){

				DE("address_0:%d, %d, %d, %d\n", address_1[j][0], address_1[j][1], address_1[j][2], address_1[j][3]);
				s1.y = address_1[j][1] + selection_1[i][0];
				s1.x = address_1[j][0];
				s2.y = address_1[j][1] + selection_1[i][1];
				s2.x = address_1[j][2];
				DE("address:%d, %d, %d, %d\n", s1.x, s1.y, s2.x, s2.y);


				pic_cutting_1(img, img_5, s1, s2);
				box_flag = box_selection(img_5);
				DE("box_flag:%d\n", box_flag);

				if(box_flag > 5){
					rectangle(img, s1, s2, color, 2, 8, 0 );
					sprintf(str, "%d", j);
					namedWindow(str);
					imshow(str, img_5);
				}
			}
		}
	}
	
	namedWindow("img");
	imshow("img",img);

	std::cout<<"ok!"<<std::endl;
	waitKey(0);
	return 0;

}

