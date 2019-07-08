#include <iostream>
#include "opencv2/opencv.hpp"
#include "Moravec.h"
#include "Describe.h"
#include "Ransac.h"
#include "Merge.h"

using namespace std;
using namespace cv;

int main() {
	Mat src1, src2, color_src1, color_src2;
	Mat mravec_dst1, mravec_dst2, hog_dst1, hog_dst2, ran_dst, plate;
	/// Load image
	src1 = imread("girls_1.jpg", IMREAD_GRAYSCALE);
	src2 = imread("girls_2.jpg", IMREAD_GRAYSCALE);
	color_src1 = imread("girls_1.jpg", IMREAD_COLOR);
	color_src2 = imread("girls_2.jpg", IMREAD_COLOR);

	Moravec moravaec1(src1);
	Moravec moravaec2(src2);
	moravaec1.CreateC(src1, 15000);
	moravaec2.CreateC(src2, 15000);

	cvtColor(src1, mravec_dst1, COLOR_GRAY2BGR);
	cvtColor(src2, mravec_dst2, COLOR_GRAY2BGR);
	cvtColor(src1, hog_dst1, COLOR_GRAY2BGR);
	cvtColor(src2, hog_dst2, COLOR_GRAY2BGR);
	ran_dst.create(src2.size(), CV_8UC1);
	moravaec1.DrawFeature(mravec_dst1);
	moravaec2.DrawFeature(mravec_dst2);

	Describe describe1;
	Describe describe2;

	describe1.make_describe_keypoints(src1, moravaec1.get_key_points(), 16);
	describe2.make_describe_keypoints(src2, moravaec2.get_key_points(), 16);

	describe1.match_descriptors(describe1.get_describe_keypoints(), describe2.get_describe_keypoints(), 0.7);
	describe1.DrawFeature(hog_dst1, hog_dst2, moravaec1.get_key_points(), moravaec2.get_key_points());

	Ransac r;
	r.do_ransac(moravaec1.get_key_points(), moravaec2.get_key_points(), describe1.get_matches(), 1);
	Merge::BackwardingMapping(src2, ran_dst, r.getH());
	Merge::BackwardingMappingColor(color_src1, color_src2, plate, r.getH());
	imshow("mravec_dst1", mravec_dst1);
	imshow("mravec_dst2", mravec_dst2);
	imshow("hog_dst1", hog_dst1);
	imshow("hog_dst2", hog_dst2);
	imshow("ran_dst", ran_dst);
	imshow("result", plate);
	waitKey();

	return 0;
}