#pragma once
#include "opencv2/opencv.hpp"
#include <algorithm> 
#include <queue>
#include <utility> 
#include <vector>

class Keypoint
{

};
class Moravec
{
private:
	cv::Mat c;
	std::vector<cv::Point> key_points;

public:
	Moravec(const cv::Mat &src);
	void CreateC(const cv::Mat &src, float threshold);
	void non_max_suppression(float threshold);
	void DrawFeature(const cv::Mat &dst);
	std::vector<cv::Point> get_key_points();
};