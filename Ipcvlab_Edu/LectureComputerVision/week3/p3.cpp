#include "opencv2\opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(){
	Mat src, dst;
	int threshold = 80;

	src = imread("lena_color.jpg",CV_LOAD_IMAGE_GRAYSCALE);

	if(src.empty()){
		cout << "파일이 없습니다. " << endl;
		return -1;
	}

	dst.create(Size(src.rows, src.cols), CV_8UC1);

	//이진화 알고리즘을 구현하시오.
	for(int y=0; y<src.rows;y++){
		for(int x=0; x<src.cols;x++){
			if(src.at<uchar>(y,x) > 100){
				dst.at<uchar>(y,x) = 255;
				continue;
			}
			dst.at<uchar>(y,x) = 0;
		}
	}

	imshow("original", src);
	imshow("result", dst);
	cvWaitKey();
	return 0;
}