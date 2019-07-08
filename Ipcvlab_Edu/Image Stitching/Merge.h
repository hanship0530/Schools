#pragma once
#include "opencv2/opencv.hpp"
namespace Merge {
	void MatrixMul(float homoCoord[1][3], float outputCoord[1][3], cv::Mat H);
	void BackwardingMapping(cv::Mat &src, cv::Mat &dst, cv::Mat H);
	void BackwardingMappingColor(cv::Mat &src1, cv::Mat &src2, cv::Mat &plate, cv::Mat H);
}