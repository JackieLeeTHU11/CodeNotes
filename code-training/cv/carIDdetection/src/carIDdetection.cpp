//============================================================================
// Name        : carIDdetection.cpp
// Author      : Jackie Lee
// Version     : V0.1
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
///
//#include <iostream>
//using namespace std;
//
//int main() {
//	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
//	return 0;
//}

//#include <iostream>
//#include <cv.h>
//#include <highgui.h>
//using namespace cv;
//using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>
#include <string.h>
#include <opencv/cv.h>
#include <stdio.h>
//#include "lib/normal.h"
using namespace cv;
using namespace std;

void pic_gray(Mat& mat1, Mat& mat2){
    IplImage pI = mat1;
    uchar* ptr;
    CvScalar s;

    int width = mat1.rows;
    int height = mat1.cols;

    mat2 = cv::Mat(width, height, CV_8UC1, 1);
    ptr = mat2.ptr(0);
    for(int i = 0; i < width; i++){
        for(int j=0; j<height; j++){
            s = cvGet2D(&pI,i,j);
            int grayScale = (int)(s.val[0]*0.299 + s.val[1]*0.587 + s.val[2]*0.114);
            ptr[i*height+j] = grayScale;
        }
    }
}
int histogram_Calculate(Mat& mat1, int number){
    Mat gray_hist;
    int histSize = 255;
    float range[] = { 0, 255 } ;
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;
    int width, height;
    int i, j;
    uchar* ptr = mat1.ptr(0);
    long int pixel_all = 0, pixel_Calc = 0;

    calcHist(&mat1, 1, 0, Mat(), gray_hist, 1, &histSize, &histRange, uniform, accumulate);

    width = gray_hist.rows;
    height = gray_hist.cols;

    for(i=0; i<=width; i++){
        pixel_all += ptr[i];
    }

    for(i=0; i<=width; i++){
        pixel_Calc += ptr[255 - i];
        if(((pixel_Calc * 100) / pixel_all) > number){
            i = 255 - i;
            break;
        }
    }
    return i;
}

void pic_Thresholding(Mat& mat1, int threshold){
    uchar* ptr = mat1.ptr(0);
    int width = mat1.rows;
    int height = mat1.cols;

    for(int i = 0; i < width; i++){
        for(int j=0;j<height;j++){
            if(ptr[i*height+j] > 125){
                ptr[i*height+j] = 255;
            }else{
                ptr[i*height+j] = 0;
            }
        }
    }
}

int** selection_Function_1(Mat& mat1, int* number){
    int **a, i, j, flag, num = 0, enter_flag = 0;
    int width = mat1.rows;
    int height = mat1.cols;
    uchar* ptr = mat1.ptr(0);

    a = (int**)malloc(width * sizeof(int*));

    for(i=0; i<width; i++){
        flag = 0;
        for(j=0; j< height-1; j++){
            if(ptr[i*height + j] != ptr[i*height + j +1]){
                flag += 1;
            }
        }
        if((flag >= 7) && (enter_flag == 0)){
            a[num] = (int* )malloc(2 * sizeof(int));
            a[num][0] = i;
            enter_flag = 1;
        }else if((enter_flag != 0) && (flag < 7)){
            if(i - a[num][0] < 8){
                continue;
            }
            a[num][1] = i - 1;
            num ++;
            enter_flag = 0;
        }
    }
    *number = num;
    return a;
}
void pic_cutting(Mat& mat1, Mat& pic_cutting, int** selection, int number){
    int real_height = mat1.cols;
    IplImage pI_1 = mat1;
    IplImage pI_2;
    IplImage pI_3;
    CvScalar s;

    pic_cutting = cv::Mat(selection[number][1] - selection[number][0], real_height, CV_8UC3, 1);
    pI_2 = pic_cutting;

    for(int i = selection[number][0]; i < selection[number][1]; i++){
        for(int j=0; j<real_height; j++){
            s = cvGet2D(&pI_1, i, j);
            cvSet2D(&pI_2, i-selection[number][0], j, s);
        }
    }
}

int choice_Color(Mat& mat1, int color_Start, int color_End){
    int width = mat1.rows;
    int height = mat1.cols;
    uchar* ptr = mat1.ptr(0);
    IplImage pI_1;
    int flag[width];
    int num, i, j, num_width = 0;
    CvScalar s;

    pI_1 = mat1;
    cvCvtColor(&pI_1, &pI_1, CV_BGR2HSV);
    for(i=0; i<width; i++){
        num = 0;
        for(j=0; j<height; j++){
            s = cvGet2D(&pI_1, i, j);
            if((s.val[0] >= color_Start) && (s.val[0] <= color_End)){
                num += 1;
            }
        }
        if(num > 20){
            flag[i] = 1;
            num_width += 1;
        }else{
            flag[i] = 0;
        }
        num = 0;
    }
    return num_width;
}


int** car_License_box(Mat& mat1, Mat& mat2, int* number){
    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Point s1, s2;
    int width_1, height_1;
    int width = mat1.rows;
    int height = mat1.cols;
    int sum = 0;

    int morph_elem = 3;
    int morph_size = 3;

    int** a = (int**)malloc(width * sizeof(int*));

    //腐蚀
    Mat element = getStructuringElement(MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( -1, -1));
    dilate(mat1, mat1, element);

    /// 找到轮廓
    findContours(mat1, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// 多边形逼近轮廓 + 获取矩形和圆形边界框
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );

    for( int i = 0; i < contours.size(); i++ )
    { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        minEnclosingCircle( contours_poly[i], center[i], radius[i] );
    }

    /// 画多边形轮廓 + 包围的矩形框 + 圆形框
    mat2 = Mat::zeros(mat1.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        s1 = boundRect[i].tl();
        s2 = boundRect[i].br();
        height_1 = s2.x - s1.x;
        width_1 =  s2.y - s1.y;

        if((height_1 > (3 * width_1)) && (width_1 > (width / 2))){
            a[sum] = (int* )malloc(4 * sizeof(int));
            a[sum][0] = s1.x;
            a[sum][1] = s1.y;
            a[sum][2] = s2.x;
            a[sum][3] = s2.y;
            sum += 1;
        }
    }
    *number = sum;
    return a;
}

void pic_cutting_1(Mat& mat1, Mat& mat2, Point s1, Point s2){
    int i, j;
    IplImage pI_1;
    IplImage pI_2;
    CvScalar s;

    mat2 = cv::Mat(s2.y - s1.y, s2.x - s1.x, CV_8UC3, 1);
    pI_1 = mat1;
    pI_2 = mat2;

    for(i = s1.y; i < s2.y; i++){
        for(j=s1.x; j<s2.x; j++){
            s = cvGet2D(&pI_1, i, j);
            cvSet2D(&pI_2, i-s1.y, j-s1.x, s);
        }
    }
}

int box_selection(Mat& mat1){
    int width_1, height_1;
    int width = mat1.rows;
    int height = mat1.cols;
    int i, j;

    IplImage pI_1 = mat1;

    CvScalar s;
    int find_blue = 0;
    int blueToWhite = 0;
    int sum =0;

    for(i=0; i<width; i++){
        find_blue = 0;
        blueToWhite = 0;
        for(j=0; j<height; j++){
            s = cvGet2D(&pI_1, i, j);
            if((s.val[0] - s.val[1] > 10) && (s.val[0] - s.val[2] > 10) && (s.val[1] < 150) && (s.val[2] < 150)){
                find_blue = 1;
            }
            else if((s.val[1] > 150) && (s.val[2] > 150) && (s.val[0] > 150) && (find_blue == 1)){
                blueToWhite += 1;
                find_blue = 0;
            }
        }
        if(blueToWhite > 5){
            sum += 1;
        }
    }
    return sum;
}



#define DEBUG

#ifdef DEBUG
#define DE(format, ...) printf(format, ## __VA_ARGS__)
#else
#define DE(format, ...) while(0)
#endif

int main(int argc,char *argv[]){
    int threshold = 0;
//    Mat img = cv::imread(argv[1]);
    Mat img = imread("carid4.jpg");
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
    // 读图
    // 图像灰阶/二值化
    namedWindow("img");
    imshow("img",img);
    pic_gray(img, img_2);

    threshold = histogram_Calculate(img_2, 5);
    DE("threshold:%d\n",threshold);

    pic_Thresholding(img_2, threshold);

    namedWindow("tmp");
    imshow("tmp", img_2);

    //图像分割
    /*
     车牌是使用是蓝底白字，而这个二值化图像img_2中，蓝色的背景被去除了，留下了白色的字。
     而根据车牌的特点，每一行中蓝色和白色的交替应该至少是7次。
     转化在这个img_2的二值图像中，则表示在车牌所在的行中，像素值的跳变至少是7次。
     所以根据这个特性，可以将图像中，可能是车牌号所在位置的图像分割出来。使用的代买如下：
     * */
    selection_1 = selection_Function_1(img_2, &selection_Number_1);
	for(i=0; i< selection_Number_1; i++){
		DE("selection_1[%d]:%d, %d\n", i, selection_1[i][0], selection_1[i][1]);
	}

	for(i=0; i<selection_Number_1; i++){
		pic_cutting(img, img_3, selection_1, i);
		sprintf(str, "%d", i);
		namedWindow(str);
		imshow(str, img_3);
	}
        // 图片筛选
        /*从车牌号特性知道，车牌的背景为蓝色。
         * 这里就是检测分割出来的图片中，蓝色占的数量，将不满足要求的图片抛弃掉。
         * 接着将筛选出来的图片，再一次灰阶、二值化。
         * */
        for(i=0; i<selection_Number_1; i++){
                pic_cutting(img, img_3, selection_1, i);
                color_num = choice_Color(img_3, 110, 120);   //蓝色为H:110--120
                DE("color_num:%d\n", color_num);
                if(color_num > 5){
                    IplImage pI_1 = img_3;
                    cvCvtColor(&pI_1, &pI_1, CV_HSV2BGR);

                    pic_gray(img_3, img_3);
                    threshold = histogram_Calculate(img_3, 3);
                    pic_Thresholding(img_3, threshold);

                    sprintf(str, "%d", i);
                    namedWindow(str);
                    imshow(str, img_3);
                    // 矩形检测
                    /*
                     * 因为车牌是一个矩形。所以接着将又一次二值化之后的图片，进行膨胀，之后在进行矩形检测。
                     * 框选出可能是车牌号的矩形区域。
                     * */
                    pic_gray(img_3, img_3);
					threshold = histogram_Calculate(img_3, 3);
					cout<<"thresholding is:"<<threshold<<endl;
					pic_Thresholding(img_3, threshold);

					address_1 = car_License_box(img_3, img_4, &address_Number_1);

					sprintf(str, "%d", i);
					namedWindow(str);
					imshow(str, img_3);
					// 矩形分割
					/*
					 * 上面矩形分割，已经将可能是车牌的矩形位置角点保存在了address_1中，这里我们根据address_1中的角点坐标，
					 * 将对应的矩形从原图像中复制到新图像中显示：
					 * */
					address_1 = car_License_box(img_3, img_4, &address_Number_1);

					for(j=0; j< address_Number_1; j++){

						DE("address_0:%d, %d, %d, %d\n", address_1[j][0], address_1[j][1], address_1[j][2], address_1[j][3]);
						s1.y = address_1[j][1] + selection_1[i][0];
						s1.x = address_1[j][0];
						s2.y = address_1[j][1] + selection_1[i][1];
						s2.x = address_1[j][2];
						DE("address:%d, %d, %d, %d\n", s1.x, s1.y, s2.x, s2.y);

						pic_cutting_1(img, img_5, s1, s2);

						sprintf(str, "%d", j);
						namedWindow(str);
						imshow(str, img_5);
					}
//					}
//                }
//            }
//
//        for(j=0; j< address_Number_1; j++){
//
//                        DE("address_0:%d, %d, %d, %d\n", address_1[j][0], address_1[j][1], address_1[j][2], address_1[j][3]);
//                        s1.y = address_1[j][1] + selection_1[i][0];
//                        s1.x = address_1[j][0];
//                        s2.y = address_1[j][1] + selection_1[i][1];
//                        s2.x = address_1[j][2];
//                        DE("address:%d, %d, %d, %d\n", s1.x, s1.y, s2.x, s2.y);
//
//                        pic_cutting_1(img, img_5, s1, s2);
//                        box_flag = box_selection(img_5);
//                        DE("box_flag:%d\n", box_flag);
//
//                        if(box_flag > 5){
//                            rectangle(img, s1, s2, color, 2, 8, 0 );
//                            sprintf(str, "%d", j);
//                            namedWindow(str);
//                            imshow(str, img_5);
                        }
                    }
	namedWindow("img");
	imshow("img",img);
    waitKey(0);
    return 0;
}
