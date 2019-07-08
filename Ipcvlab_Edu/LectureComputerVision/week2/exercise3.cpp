#include "opencv2\opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;
int main() {
	Mat mat = imread("C:\\computerVision\\lena_color.jpg",CV_LOAD_IMAGE_COLOR);
	Mat resultMat;

	if(mat.empty()){
		cout<<"wrong file name"<<endl;\
			return -1;
	}

	resultMat.create(Size(mat.rows, mat.cols), CV_8UC1);
	
	for(int y=0; y<mat.rows; y++){
		for(int x=0; x<mat.cols;x++){
			resultMat.at<uchar>(y,x) = (saturate_cast<uchar>(mat.at<Vec3b>(y,x)[0])+
			saturate_cast<uchar>(mat.at<Vec3b>(y,x)[1])+
			saturate_cast<uchar>(mat.at<Vec3b>(y,x)[2]))/3;
		}
	}
	imshow("original", mat);
	imshow("result",resultMat);
	cvWaitKey();
}
