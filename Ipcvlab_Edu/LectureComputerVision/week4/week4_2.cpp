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

	
	double LPF[9] = {
		//Low Pass Filter 계수를 채워넣으세요.
		1.0/9,1.0/9,1.0/9,1./9,1./9,1./9,1./9,1./9,1./9
	};

	//Filter 연산을 구현하시오.
	for(int j=1; j<img.rows-1; j++){
		for(int i=1; i<img.cols-1; i++){
			dst.at<uchar>(j,i) = (char)((double)img.at<uchar>(j-1,i-1)*LPF[0]+
				(double)img.at<uchar>(j-1,i)*LPF[1] +
				(double)img.at<uchar>(j-1,i+1)*LPF[2]+
				(double)img.at<uchar>(j,i-1)*LPF[3]+
				(double)img.at<uchar>(j,i)*LPF[4]+
				(double)img.at<uchar>(j,i+1)*LPF[5]+
				(double)img.at<uchar>(j+1,i-1)*LPF[6]+
				(double)img.at<uchar>(j+1,i)*LPF[7]+
				(double)img.at<uchar>(j+1,i+1)*LPF[8]); 
			//std::cout<< dst.at<uchar>(j,i)<<", ";
			if(dst.at<uchar>(j,i) > 255){
				dst.at<uchar>(j,i) = 255;
			}
			if(dst.at<uchar>(j,i) < 0){
				dst.at<uchar>(j,i) = 0;
			}
		}
		//std::cout<<std::endl;
	}

	imshow("orignal", img);
	imshow("result", dst);
	cvWaitKey();
}