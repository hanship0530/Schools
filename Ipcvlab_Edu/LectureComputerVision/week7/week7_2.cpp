#include "Filter.h"
#include <cmath>

using namespace cv;

void main()
{
	Mat src;	
	src = imread("lena_color.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	
	imshow("original", src);

	SobelFilterY sobelY;
	sobelY.CreateMask();
	Mat sobelYOut = sobelY.Convolute(src);

	SobelFilterX sobelX;
	sobelX.CreateMask();
	Mat sobelXOut = sobelX.Convolute(src);

	Mat EdgeStrength;
	EdgeStrength.create(Size(src.rows, src.cols), CV_8UC1);
	//EdgeStrength를 구현하시오.

	for (int h = 0; h < src.rows; h++) {
		for (int w = 0; w < src.cols; w++) {
			EdgeStrength.at<uchar>(h, w) = sqrt(pow(sobelXOut.at<uchar>(h, w), 2) + pow(sobelYOut.at<uchar>(h, w), 2));
		}
	}

	imshow("sobelY-Output", sobelYOut);
	imshow("sobelX-Output", sobelXOut);
	imshow("Edge Strength",EdgeStrength);
	cvWaitKey();

}