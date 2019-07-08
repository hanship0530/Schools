#include "opencv2\opencv.hpp"
#include <iostream>

#include "LineFitting.h"
using namespace cv;
using namespace std;


void main ()
{
	cv::Mat src(280, 280, CV_8UC1, cv ::Scalar(0));
	CRANSAC ransac;

	ransac.MakeRandomPoint();
	ransac.DrawPoint(src);


	for(int l = 0; l < 1000; ++l)
	{				
		ransac.RandomGetPT();
		ransac.CalLine();	
		ransac.CalInliner();

	}

	cv::Mat temp;
	src.copyTo(temp);
	ransac.DrawLine(temp);
	cvNamedWindow("ransac", CV_WINDOW_NORMAL);
	imshow("ransac", temp);
	cvWaitKey();
}