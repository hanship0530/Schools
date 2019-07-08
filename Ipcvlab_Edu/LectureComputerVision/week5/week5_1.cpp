#include "opencv2\opencv.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace cv;
using namespace std;

void SaltNPepper(Mat src, int amount) {

	srand((int)time(NULL));
	for (int k = 0; k < amount; k++) {
		int i = rand() % src.cols;
		int j = rand() % src.rows;

		int saltNpepper = (rand() % 2) * 255;

		src.at<uchar>(j, i) = saltNpepper;
	}
}

void MedienFilter(Mat src, Mat dst) {
	double mask[9];
	//MedienFilter
	for (int j = 1; j < src.rows-1; j++) {
		for (int i = 1; i < src.cols-1; i++) {
			mask[0] = src.at<uchar>(j-1, i-1);
			mask[1] = src.at<uchar>(j-1, i);
			mask[2] = src.at<uchar>(j-1, i+1);
			mask[3] = src.at<uchar>(j, i-1);
			mask[4] = src.at<uchar>(j, i);
			mask[5] = src.at<uchar>(j, i+1);
			mask[6] = src.at<uchar>(j+1, i-1);
			mask[7] = src.at<uchar>(j+1, i);
			mask[8] = src.at<uchar>(j+1, i+1);
			for (int y = 0; y < 8; y++) {
				for (int x = 0; x < 9-y-1; x++) {
					if (mask[x] > mask[x + 1]) {
						double temp = mask[x+1];
						mask[x + 1] = mask[x];
						mask[x] = temp;
					}
				}
			}
			dst.at<uchar>(j, i) = (unsigned char)mask[9 / 2];
		}
	}
}

int main() {

	Mat src, dst;

	src = imread("lena_color.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	dst.create(Size(src.rows, src.cols), CV_8UC1);

	SaltNPepper(src, 5000);
	MedienFilter(src, dst);

	imshow("original", src);
	imshow("dst", dst);
	cvWaitKey();
}