#include "opencv2\opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void Erosion(Mat src, Mat dst) {
	double mask[25];
	//Erosion
	for (int j = 2; j < src.rows - 2; j++) {
		for (int i = 2; i < src.cols - 2; i++) {
			int index = 0;
			for (int k = 0; k < 5; k++) {
				for (int m = 0; m < 5; m++) {
					mask[index++] = src.at<uchar>(j - 2 + k, i - 2 + m);
				}
			}
			for (int y = 0; y < 25 - 1; y++) {
				for (int x = 0; x < 25 - y - 1; x++) {
					if (mask[x] > mask[x + 1]) {
						double temp = mask[x + 1];
						mask[x + 1] = mask[x];
						mask[x] = temp;
					}
				}
			}
			dst.at<uchar>(j, i) = (unsigned char)mask[0];
		}
	}

}

void Dilation(Mat src, Mat dst) {
	double mask[25];
	//Dilation
	for (int j = 2; j < src.rows - 2; j++) {
		for (int i = 2; i < src.cols - 2; i++) {
			int index = 0;
			for (int k = 0; k < 5; k++) {
				for (int m = 0; m < 5; m++) {
					mask[index++] = src.at<uchar>(j - 2 + k, i - 2 + m);
				}
			}
			for (int y = 0; y < 25 - 1; y++) {
				for (int x = 0; x < 25 - y - 1; x++) {
					if (mask[x] > mask[x + 1]) {
						double temp = mask[x + 1];
						mask[x + 1] = mask[x];
						mask[x] = temp;
					}
				}
			}
			dst.at<uchar>(j, i) = (unsigned char)mask[24];
		}
	}
}

void Opening(Mat src, Mat dst) {
	Mat temp;
	temp.create(Size(src.rows, src.cols), CV_8UC1);
	//Opening
	Erosion(src, temp);
	Dilation(temp, dst);

}

void Closing(Mat src, Mat dst) {
	Mat temp;
	temp.create(Size(src.rows, src.cols), CV_8UC1);
	//Closing
	Dilation(src, temp);
	Erosion(temp, dst);

}

int main() {

	Mat src, dst, dst2, dst3, dst4;

	src = imread("morphologyTest.jpg", CV_LOAD_IMAGE_GRAYSCALE);


	dst.create(Size(src.rows, src.cols), CV_8UC1);
	Dilation(src, dst);

	dst2.create(Size(src.rows, src.cols), CV_8UC1);
	Erosion(src, dst2);

	dst3.create(Size(src.rows, src.cols), CV_8UC1);
	Closing(src, dst3);

	dst4.create(Size(src.rows, src.cols), CV_8UC1);
	Opening(src, dst4);

	imshow("original", src);
	imshow("Dilation", dst);
	imshow("Erosion", dst2);

	imshow("Closing", dst3);
	imshow("Opening", dst4);
	cvWaitKey();
}