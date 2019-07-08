#include "opencv2\opencv.hpp"
#include <iostream>

#include "LineFitting.h"
using namespace cv;
using namespace std;


void main ()
{
	cv::Mat src(280, 280, CV_8UC1, cv ::Scalar(0));
	LeastSquare LS;

	LS.MakeRandomPoint();
	LS.DrawPoint(src);
	LS.CalLine();

	cv::Mat temp;
	src.copyTo(temp);
	LS.DrawLine(temp);
	cvNamedWindow("LS", CV_WINDOW_NORMAL);
	imshow("LS", temp);
	cvWaitKey();
}