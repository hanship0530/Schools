#include "LogFilter.h"

using namespace cv;
void main()
{
	Mat src;	
	/// Load image
	src = imread("lena_color.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	
	imshow("original", src);	

	double sigma = 2.0;
	LOG log;
	log.CreateMask(sigma);
	log.Convolute(src);
	cv::Mat logOut;
	logOut.create(src.size(), CV_8UC1);
	logOut.setTo(Scalar(0.0));
	log.FindZeroCrossing(logOut);

	imshow("log-Output", logOut);
	cvWaitKey();

}