/*
 * lanedetector.h
 *
 *  Created on: 2017-3-20
 *      Author: lulu
 */

#ifndef LANEDETECTOR_H_
#define LANEDETECTOR_H_
#include"lanetracker.h"
#include"segmentation.h"
#include<vector>
using namespace cv;
using namespace std;
class LaneDetector{
private:
	LaneTracker egotr;
	LaneTracker lefttr;
	LaneTracker righttr;
	int l_index,lastloop_l;
	int r_index,lastloop_r;
	vector<Vec4i> lanelist;
	float dt;


public:
	bool leftpreflag,rightpreflag,egopreflag;
	vector<Vec3f> egolanetrack, leftlanetrack, rightlanetrack;
	LaneDetector():egolanetrack(2), leftlanetrack(1), rightlanetrack(1){
		egotr = LaneTracker(2, 0.1,500);
		lefttr = LaneTracker(1, 0.1, 500);
		righttr = LaneTracker(1, 0.1, 500);
		dt = 0.1;
		for(int l = 0; l <6; ++l){lanelist.push_back(Vec4i(0,0,0,0));}
		l_index = 0;
		lastloop_l = 0;
		r_index = 0;
		lastloop_r = 0;
		leftpreflag = false;
		rightpreflag = false;
		egopreflag = false;
//						rightflag.push_back(0);
//						egoflag.push_back(0);
//						egoflag.push_back(0);
//						leftflag.push_back(0);
	}
	void lanedetection(vector<Mat> &img, int thr, int imgloop);
	void figout_points(Mat &img1,vector<vector<Point2f> > &lanepoints, int lanewidth);
	void figout(Mat &img1);


};

float dista(Vec3f &a, Vec3f &b);
#endif /* LANEDETECTOR_H_ */
