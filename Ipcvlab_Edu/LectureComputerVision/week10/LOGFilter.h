#pragma once
#include "opencv2\opencv.hpp"
class Filter
{
protected:
	int maskSize;
	double **mask;
	void AllocateMask();
	cv::Mat logReult;
public:
	Filter()  : mask(NULL){}
	~Filter();
	virtual void CreateMask() = 0;
	void Convolute(const cv::Mat &org);
};

class LOG : public Filter
{
private:
	void CreateMask(){}	
public:
	void CreateMask(double sigma);
	void FindZeroCrossing(cv::Mat &out);
	int CheckSign(float val1, float val2);
};