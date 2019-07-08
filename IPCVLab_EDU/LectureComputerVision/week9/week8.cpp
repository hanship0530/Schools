#include "Moravec.h"
using namespace cv;
void main()
{
	Mat src;	
	/// Load image
	src = imread("bucks.jpg", CV_LOAD_IMAGE_GRAYSCALE);	
	Moravec moravaec(src);
	moravaec.CreateC(src);

	Mat dst;
	cvtColor(src, dst, CV_GRAY2BGR);
	moravaec.DrawFeature(dst);

	imshow("src", src);
	imshow("dst", dst);
	cvWaitKey();

}