//https://www.learnopencv.com/histogram-of-oriented-gradients/
#pragma once
#include "opencv2/opencv.hpp"
#include <algorithm> 
#include <queue>
#include <utility> 
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstring>
#define PI 3.14159265

using namespace std;
using namespace cv;
class Describe
{
private:
	vector<pair<int, int>> matches;
	vector<pair<Point, vector<float>>> describe_keypoints;
public:
	Describe();
	bool sortbysec(const pair<int, float> &a, const pair<int, float> &b);
	void make_describe_keypoints(const cv::Mat &src, const std::vector<cv::Point> &key_points, const int patch_size);
	void match_descriptors(vector<pair<Point, vector<float>>> desc1, vector<pair<Point, vector<float>>> desc2, float threshold);
	void DrawFeature(const cv::Mat &dst1, const cv::Mat &dst2, const vector<Point> &key_point1, const vector<Point> &key_point2);
	vector<pair<Point, vector<float>>> get_describe_keypoints();
	vector<pair<int, int>> get_matches();
};

// class Descriptor {
// public:
// 	Descriptor();
// 	void match_descriptors
// };