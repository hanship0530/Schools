#include "opencv2\opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int sdaadsa() {
	double subMat[2][2] = {{1,2},{3,4}};
	Mat mat = cv::Mat(2,2,CV_64FC1,subMat);
	//mat.create(Size(2, 2), CV_8UC1);
	cout<<"mat"<<endl;
	cout<<"[";
	int y,x;
	for(y=0; y<mat.rows; y++){
		for(x=0; x<mat.cols;x++){
			if(x==mat.cols-1){
				cout<<mat.at<double>(y,x);
			}else {
				cout<<mat.at<double>(y,x)<<", ";
			}
		}
		if(y==mat.rows-1){
			cout<<"]";
		}else {
			cout<<endl;
		}
	}
	imshow("original", mat);
	cvWaitKey();
	return 0;
}
