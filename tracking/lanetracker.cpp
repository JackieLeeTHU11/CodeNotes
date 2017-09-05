/*
 * lanetracker.cc
 *
 *  Created on: 2017-3-17
 *      Author: lulu
 */
#include"lanetracker.h"

void LaneTracker::renew(int lanenum, float proc_noise_scale, float meas_noise_scale)
{
	this->n_lanes = lanenum;
	this->meas_size = lanenum*3;
	this->state_size = this->meas_size*2;
	kf.init(this->state_size, this->meas_size, 0);
	kf.measurementMatrix = Mat::zeros(this->meas_size, this->state_size, CV_32FC1);
	kf.transitionMatrix = Mat::eye(this->state_size, this->state_size,CV_32FC1);
	for(int p = 0; p <this->meas_size; ++p ){kf.measurementMatrix.at<float>(p, p*2) = 1;}
//		cout<<kf.measurementMatrix<<endl;
	for(int p = 0; p < kf.processNoiseCov.rows; p += 2)
	{
		kf.processNoiseCov.at<float>(p,p) = 0.25;
		kf.processNoiseCov.at<float>(p+1,p+1) = 1.;
		kf.processNoiseCov.at<float>(p,p+1) = 0.5;
		kf.processNoiseCov.at<float>(p+1,p) = 0.5;
	}
	this->kf.measurementNoiseCov = Mat::eye(this->meas_size,this->meas_size,CV_32FC1) * meas_noise_scale;
	this->kf.errorCovPre = Mat::eye(this->state_size, this->state_size, CV_32FC1);
	this->meas = Mat::zeros(this->meas_size, 1, CV_32FC1);
	this->state =Mat::zeros(this->state_size, 1, CV_32FC1);
	this->first_detected = false;
}

void LaneTracker::updatedt(float dt)
{
	for(int i = 0; i < this->state_size; i = i +2)
	{
		this->kf.transitionMatrix.at<float>(i, i+1) = dt;

	}
}

void  LaneTracker::_first_detect(vector<Vec3f> &lanes){
	for(int i = 0 ; i < lanes.size(); ++i)
	{
		for(int j = 0; j <6; j = j + 2)
		{
			this->state.at<float>(j+ i*6, 0) = lanes[i][(int)j/2];
		}

	}
	this->kf.statePost = this->state;
	this->first_detected = true;
}

void LaneTracker::update(vector<Vec3f> &lanes,vector<int> &flag){
	if(this->first_detected)
	{
//		cout<<"first detected test!"<<endl;
		for(int i = 0 ; i < (int)lanes.size(); ++i)
		{
//			cout<<"flag:"<<flag[i]<<endl;
			if(flag[i]>0)
			{
				for(int j = 0; j <3; ++j )
				{
					this->meas.at<float>(j+ i*3, 0) = lanes[i][j];
				}
			}
		}
		this->kf.correct(this->meas);
//		cout<<"state:"<<this->state<<endl;
	}
	else{
		int sum= accumulate(flag.begin() , flag.end() , 0);
//		for(int p = 0; p <flag.size(); p ++){sum+=flag[p];}
		if(sum==flag.size()){
			this->_first_detect(lanes);
		}
	}
}
bool LaneTracker::predict(float dt, vector<Vec3f> &lanestrack)
{
	if(this->first_detected)
	{
		this->updatedt(dt);
		Mat prediction = this->kf.predict();
		for(int o = 0; o < prediction.rows; o = o +6)
		{
			lanestrack[(int)o/6] = Vec3f(prediction.at<float>(o), prediction.at<float>(o+2), prediction.at<float>(o+4));
		}
		return true;
	}
	else
	{
		return false;
	}
}


