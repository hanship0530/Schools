#include "opencv2\opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;
int sadsa() {
	Mat mat = imread("C:\\computerVision\\lena_color.jpg",CV_LOAD_IMAGE_COLOR);
	Mat copyMat;

	if(mat.empty()){
		cout<<"wrong file name"<<endl;\
			return -1;
	}

	copyMat.create(Size(mat.rows, mat.cols), CV_8UC3);
	
	for(int y=0; y<mat.rows; y++){
		for(int x=0; x<mat.cols;x++){
			copyMat.at<Vec3b>(y,x)[0] = 255 - saturate_cast<uchar>(mat.at<Vec3b>(y,x)[0]);
			copyMat.at<Vec3b>(y,x)[1] = 255 - saturate_cast<uchar>(mat.at<Vec3b>(y,x)[1]); 
			copyMat.at<Vec3b>(y,x)[2] = 255 - saturate_cast<uchar>(mat.at<Vec3b>(y,x)[2]); 
		}
	}
	imshow("original", mat);
	imshow("inverseMat",copyMat);
	cvWaitKey();
}
