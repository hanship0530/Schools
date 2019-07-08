#include "opencv2\opencv.hpp"

using namespace cv;

int main(){
	Mat img = imread("lena_color.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat dst;

	if(img.empty()){
		std::cout << "파일이 없습니다. " << std::endl;
		return -1;
	}
	
	dst.create(Size(img.rows, img.cols), CV_8UC1);

	
	double HPF[9] = {
		//High Pass Filter 계수를 채워넣으세요.
		-1.,-1.-1.,
		-1.,8.,-1.,
		-1.,-1.,-1.
	};

	for(int j=1; j<img.rows-1; j++){
		for(int i=1; i<img.cols-1; i++){
			double sum;
			sum = (img.at<uchar>(j-1,i-1)*HPF[0]+
				img.at<uchar>(j-1,i)*HPF[1] +
				img.at<uchar>(j-1,i+1)*HPF[2]+
				img.at<uchar>(j,i-1)*HPF[3]+
				img.at<uchar>(j,i)*HPF[4]+
				img.at<uchar>(j,i+1)*HPF[5]+
				img.at<uchar>(j+1,i-1)*HPF[6]+
				img.at<uchar>(j+1,i)*HPF[7]+
				img.at<uchar>(j+1,i+1)*HPF[8]);
			if(sum<0){
				sum=0;
			}
			if(sum>255){
				sum=255;
			}
			dst.at<uchar>(j,i) = sum;
		}
	}

	imshow("orignal", img);
	imshow("result", dst);
	cvWaitKey();
}