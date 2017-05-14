#ifndef __NORMAL_H__
#define __NORMAL_H__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <string.h>
#include <opencv/cv.h>
#include <stdio.h>

using namespace cv;

/*将string转化为double*/
double char2num(char *s);

/*图像灰度化*/
void pic_gray(Mat& mat1, Mat& mat2);

/*图像二值化*/
void pic_Thresholding(Mat& mat1, int threshold);

/*Soble边沿检测*/
void pic_Sobel(Mat& mat1, double x_beta);

/*直方图计算*/
int histogram_Calculate(Mat& mat1, int number);

/*筛选步骤1*/
int** selection_Function_1(Mat& mat1, int* number);

/*返回第一步裁减分割后图片*/
void pic_cutting(Mat& mat1, Mat& pic_cutting, int** selection, int number);

/*裁减出一张图片*/
void pic_cutting_1(Mat& mat1, Mat& mat2, Point s1, Point s2);

/*通过HSV颜色空间来检测该图片是否是车牌所在图片*/
int choice_Color(Mat& mat1, int color_Start, int color_End);

/*在检测出车牌的图片中，框选车牌*/
int** car_License_box(Mat& mat1, Mat& mat2, int* number);

/*在框选出的方框中筛选出可能是车牌的方框*/
int box_selection(Mat& mat1);

#endif

