#include "opencv2\opencv.hpp"
#include "Geometry.h"

using namespace cv;

void LPF(Mat src, Mat dst){

	double mask[9] = {
		1./9, 1./9, 1./9,
		1./9, 1./9, 1./9,
		1./9, 1./9, 1./9
	};
	for(int h = 1; h < src.rows - 1; h++){
		for(int w = 1; w < src.cols - 1; w++){

			double sum = 0;
			int idx = 0;

			for(int x = -1; x <= 1; x++){
				for(int y = -1;y <= 1; y++){
					sum += mask[idx] * src.at<uchar>(h + x,w + y);

					idx++;
				}
			}
			dst.at<uchar>(h,w) = cv::saturate_cast<uchar>(sum);
		}
	}
}


int main(){

	Mat src, dst, dst2;

	src = imread("lena_color.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	dst.create(src.size(), CV_8UC1);
	dst2.create(src.size(), CV_8UC1);

	if(src.empty()){
		std::cout << "파일이 없습니다. " << std::endl;
		return -1;
	}

	CGeometry geometry;	

	// Rotate
	geometry.Move(-(src.rows/2), -(src.cols/2));
	geometry.Inverse_Rotate(70);
	geometry.Move(src.rows/2, src.cols/2);	

	geometry.BackwardingMapping(src, dst);
	LPF(dst, dst2);

	imshow("src Demo", src);
	imshow("Geometry Backwarding Mapping Demo", dst);
	imshow("LPF Geometry Backwarding Mapping Demo", dst2);
	waitKey(0);

	return 0;
}