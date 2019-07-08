#include "opencv2\opencv.hpp"
#include <iostream>
using namespace cv;

void SimplePynamid(const int Q, const int Width, const int Height, 
	const Mat &src, std::vector<Mat>& vec)
{
	int newWidth = Width;
	int newHeight = Height;

	Mat temp;
	src.copyTo(temp);
	for (int q = 0; q < Q; ++q)
	{
		Mat dst(cv::Size(newWidth / 2, newHeight / 2), CV_8UC1);

		for (int h = 0; h < dst.rows; ++h)
		{
			for (int w = 0; w < dst.cols; ++w)
			{
				dst.at<uchar>(h, w) = temp.at<uchar>(h * 2, w * 2);
			}
		}
		vec.push_back(dst);
		dst.copyTo(temp);

		newWidth = newWidth/2;
		newHeight = newHeight / 2;
	}
}

void WeightPyramid(const int Q, const int Width, const int Height,
	const Mat &src, std::vector<Mat>& vec)
{
	//WeightPyramid를 구현하시오.
	int newWidth = Width;
	int newHeight = Height;
	double weight_filter[5][5] = {
	{0.0025,0.0125,0.02,0.125,0.0025},
	{0.0125,0.0625,0.1,0.0625,0.0125},
	{0.02,0.1,0.16,0.1,0.02},
	{0.0125,0.0625,0.1,0.0625,0.0125},
	{0.0025,0.0125,0.02,0.0125,0.0025}
	};
	
	double tempd = .0;
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 5; i++) {
			tempd += weight_filter[j][i];
		}
	}
	std::cout << tempd << std::endl;
	
	Mat temp;
	src.copyTo(temp);
	for (int q = 0; q < Q; ++q)
	{
		Mat dst(cv::Size(newWidth / 2, newHeight / 2), CV_8UC1);

		for (int h = 0; h < dst.rows; ++h)
		{
			for (int w = 0; w < dst.cols; ++w)
			{
				double sum = 0;
				for (int j = 0; j < 5; j++) {
					for (int i = 0; i < 5; i++) {
						int y = h * 2 + j - 2;
						int x = w * 2 + i - 2;
						if (y >= 0 && y < temp.rows && x >= 0 && x < temp.cols)
 							sum += weight_filter[j][i] * temp.at<uchar>(y, x);
					}
				}
				if (sum > 255) {
					sum = 255;
				}
				dst.at<uchar>(h, w) = sum;
			}
		}		
		vec.push_back(dst);
		dst.copyTo(temp);

		newWidth = newWidth / 2;
		newHeight = newHeight / 2;
	}

}
void main()
{
	Mat src;	
	std::vector<Mat> Octave_Image;
	src = imread("lena_color.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	//SimplePynamid(4, src.rows, src.cols, src, Octave_Image);
	WeightPyramid(4, src.rows, src.cols, src, Octave_Image);

	int idx = 0;
	for (auto& v : Octave_Image)
	{
		std::string str = std::to_string(idx++);
		cvNamedWindow(str.c_str(), CV_WINDOW_NORMAL);
		imshow(str.c_str(), v);
	}
	cvWaitKey();
}