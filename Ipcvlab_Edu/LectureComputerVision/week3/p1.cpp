#include "opencv2\opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(){

	Mat src, YCrCbMat, OpenCV_YCrCbMat;

	src = imread("lena_color.jpg",CV_LOAD_IMAGE_COLOR);

	if(src.empty()){
		std::cout << "파일이 없습니다. " << std::endl;
		return -1;
	}
	
	YCrCbMat.create(Size(src.rows, src.cols), CV_8UC3);
	
	//BGR -> YCrCb를 구현하시오.
	for(int j=0; j<src.rows;j++){
		for(int i=0;i<src.cols;i++){
			float B = src.at<Vec3b>(j,i)[0];
			float G = src.at<Vec3b>(j,i)[1];
			float R = src.at<Vec3b>(j,i)[2];
			YCrCbMat.at<Vec3b>(j,i)[0] = 0 + 0.299*R+0.587*G+0.114*B;
			YCrCbMat.at<Vec3b>(j,i)[1] = 128 +0.5*R-0.419*G-0.081*B;
			YCrCbMat.at<Vec3b>(j,i)[2] = 128 -0.169*R-0.331*G+0.5*G;
		}
	}

	//OpenCV 함수를 사용한 BGR -> YCrCb
	cvtColor(src, OpenCV_YCrCbMat, CV_BGR2YCrCb);

	imshow("original", src);
	imshow("YCrCb", YCrCbMat);
	imshow("OpenCV_YCrCb", OpenCV_YCrCbMat);
	cvWaitKey();
}