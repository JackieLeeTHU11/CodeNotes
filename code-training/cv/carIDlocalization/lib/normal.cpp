#include "normal.h"

double char2num(char *s){
	double d=0;
	int i,t=0;

	for (i=0;s[i]!=0 ;i++){
		if(s[i]=='.'){
			t = 10; 
		}   
		else{
			if(t==0){
				d = d*10 + (s[i]-'0');
			}   
			else{
				d = d + (double)(s[i]-'0')/t;
				t *=10;
			}   
		}   
	}   
	return d;
}

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

void pic_Sobel(Mat& mat1, double x_beta){
	Mat dst_x, dst_y;

	Sobel(mat1, dst_x, mat1.depth(), 1, 0);  
	Sobel(mat1, dst_y, mat1.depth(), 0, 1);  
	convertScaleAbs(dst_x, dst_x);  
	convertScaleAbs(dst_y, dst_y);  
	addWeighted(dst_x, x_beta, dst_y, (1 - x_beta), 0, mat1);  	
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
