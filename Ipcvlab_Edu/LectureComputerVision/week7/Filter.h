#pragma once
#include "opencv2\opencv.hpp"
class Filter
{
protected:
	int maskSize;
	double **mask;
	void AllocateMask();
public:
	Filter()  : mask(NULL){}
	~Filter();
	virtual void CreateMask() = 0;
	cv::Mat Convolute(const cv::Mat &org);
};

class SobelFilterY : public Filter
{	
public:
	void CreateMask();
};

class SobelFilterX : public Filter
{	
public:
	void CreateMask();
};