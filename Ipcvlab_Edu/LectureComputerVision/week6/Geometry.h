#pragma once
#include "opencv2\opencv.hpp"
#include <cmath>
class CGeometry
{
private:
	const double PI;
	void MatrixMul(float homoMatrix[3][3]);
	void MatrixMul(float homoCoord[1][3], float outputCoord[1][3]);
	float HMatrix[3][3]; // 동차행렬

	bool initMat;
	inline double GetDadius(const int degree)
	{
		return degree * (PI / 180.);
	}

public:
	CGeometry() : PI(acos(-1))
	{
		initMat = false;
	}
	void ForwardingMapping(cv::Mat &src, cv::Mat &dst);
	void BackwardingMapping(cv::Mat &src, cv::Mat &dst);
	void Move(const int y, const int x);
	void Rotate(const int degree);
	void Inverse_Rotate(const int degree);

	void Reset(){initMat = false;}
};
