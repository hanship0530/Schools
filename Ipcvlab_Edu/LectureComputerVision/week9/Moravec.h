#pragma once
#include "opencv2\opencv.hpp"

class Moravec
{
private:
	cv::Mat c;

public:
	Moravec(const cv::Mat &src);
	void CreateC(const cv::Mat &src);
	void DrawFeature(const cv::Mat &dst);
};