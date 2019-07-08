#include "opencv2\opencv.hpp"

using namespace cv;

void Dissolve(Mat img1, Mat img2, Mat dst, double alpa){
	//디졸브를 구현하시오.
	for(int j=0; j<img1.rows; j++){
		for(int i=0; i<img1.cols; i++){
			dst.at<Vec3b>(j,i)[0] = alpa*img1.at<Vec3b>(j,i)[0] + (1-alpa)*img2.at<Vec3b>(j,i)[0];
			dst.at<Vec3b>(j,i)[1] = alpa*img1.at<Vec3b>(j,i)[1] + (1-alpa)*img2.at<Vec3b>(j,i)[1];
			dst.at<Vec3b>(j,i)[2] = alpa*img1.at<Vec3b>(j,i)[2] + (1-alpa)*img2.at<Vec3b>(j,i)[2];
		}
	}
}

int main(){
	Mat img1 = imread("lena_color.jpg", CV_LOAD_IMAGE_COLOR);
	Mat img2 = imread("dog.jpg", CV_LOAD_IMAGE_COLOR);
	Mat dst;
	double alpa = 1.0;

	if(img1.empty() || img2.empty()){
		std::cout << "파일이 없습니다. " << std::endl;
		return -1;
	}

	dst.create(Size(img1.rows, img1.cols), CV_8UC3);

	for(int i = 0; i < 6; i++){
		Dissolve(img1, img2, dst, alpa);
		imshow("result", dst);

		if(i == 5){
			cvWaitKey();
		}
		else{ 
			cvWaitKey(10);
		}
		alpa -= 0.2;
	}
}
