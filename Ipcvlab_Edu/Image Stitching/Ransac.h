#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;
using namespace cv;

class Ransac {
private:
	vector<pair<int, int>> ransac_matches;
	Mat H;
public:
	Ransac();
	void do_ransac(const vector<Point> &keypoints1, const vector<Point> &keypoints2, vector<pair<int, int>> matches, float threshlod);
	void multiMat(Mat H, const Point p, int *returnP);
	void make_affine_matrix(vector<int> p1, vector<int> p2, Mat H, const vector<Point> &keypoints1, const vector<int> matched1, const vector<Point> &keypoints2, const vector<int> matched2);
	Mat getH();
	vector<pair<int, int>> get_ransac_matches();
};