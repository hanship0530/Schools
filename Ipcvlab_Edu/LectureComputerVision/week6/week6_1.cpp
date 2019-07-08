#include "opencv2\opencv.hpp"
#include "Geometry.h"

using namespace cv;



int main(){

	Mat src, dst;

	src = imread("lena_color.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	dst.create(src.size(), CV_8UC1);

	if(src.empty()){
		std::cout << "파일이 없습니다. " << std::endl;
		return -1;
	}

	CGeometry geometry;	

	// Rotate
	geometry.Move(-(src.rows/2), -(src.cols/2));
	geometry.Rotate(70);
	geometry.Move(src.rows/2, src.cols/2);	

	geometry.ForwardingMapping(src, dst);

	imshow("src Demo", src );
	imshow("Geometry Forwarding Mapping Demo", dst);
	waitKey(0);

	return 0;
}