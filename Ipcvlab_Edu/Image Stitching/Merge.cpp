#include "Merge.h"
#include <iostream>

void Merge::MatrixMul(float homoCoord[1][3], float outputCoord[1][3], cv::Mat H)
{
	for (int i = 0; i < 1; ++i)
		for (int j = 0; j < 3; ++j)
			for (int k = 0; k < 3; ++k)
				outputCoord[i][j] += homoCoord[i][k] * H.at<float>(k, j);
}

void Merge::BackwardingMappingColor(cv::Mat & src1, cv::Mat & src2, cv::Mat & plate, cv::Mat H)
{
	plate.create(src1.size() + src2.size(), CV_8UC3);

	for (int y = 0; y < src1.rows; ++y)
	{
		for (int x = 0; x < src1.cols; ++x)
		{
			plate.at<cv::Vec3b>(y, x)[0] = src1.at<cv::Vec3b>(y, x)[0];
			plate.at<cv::Vec3b>(y, x)[1] = src1.at<cv::Vec3b>(y, x)[1];
			plate.at<cv::Vec3b>(y, x)[2] = src1.at<cv::Vec3b>(y, x)[2];
		}
	}
	float homoCoord[1][3] = { 0., };
	for (int y = 0; y < src2.rows; ++y)
	{
		for (int x = 0; x < src2.cols; ++x)
		{
			//Make BackwardingMapping
			float outputCoord[1][3] = { 0., };
			homoCoord[0][0] = y;
			homoCoord[0][1] = x;
			homoCoord[0][2] = 1;
			MatrixMul(homoCoord, outputCoord, H);
			int yp = round(outputCoord[0][0]);
			int xp = round(outputCoord[0][1]);
			if (yp >= 0 && yp < plate.rows&&xp >= 0 && xp < plate.cols) {
				plate.at<cv::Vec3b>(yp, xp)[0] = src2.at<cv::Vec3b>(y, x)[0];
				plate.at<cv::Vec3b>(yp, xp)[1] = src2.at<cv::Vec3b>(y, x)[1];
				plate.at<cv::Vec3b>(yp, xp)[2] = src2.at<cv::Vec3b>(y, x)[2];
			}
		}
	}
}



void Merge::BackwardingMapping(cv::Mat &src, cv::Mat &dst, cv::Mat H)
{	
	std::cout << "Merge H matrix" << std::endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			std::cout << H.at<float>(i, j) << " ";
		}
		std::cout << std::endl;
	}
	/*
	H.at<float>(0, 0) = 9.80585551e-01;
	H.at<float>(0, 1) = 2.28992896e-04;
	H.at<float>(1, 0) = -3.13300808e-04;
	H.at<float>(1, 1) = 9.80776229e-01;
	H.at<float>(2, 0) = -1.56139133e-02;
	H.at<float>(2, 1) = 1.29441835e+02;
	std::cout << "Changed H matrix" << std::endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			std::cout << H.at<float>(i, j) << " ";
		}
		std::cout << std::endl;
	}
	*/
	float homoCoord[1][3] = { 0., };
	for (int y = 0; y < src.rows; ++y)
	{
		for (int x = 0; x < src.cols; ++x)
		{
			//Make BackwardingMapping
			float outputCoord[1][3] = { 0., };
			homoCoord[0][0] = y;
			homoCoord[0][1] = x;
			homoCoord[0][2] = 1;
			MatrixMul(homoCoord, outputCoord, H);
			int yp = round(outputCoord[0][0]);
			int xp = round(outputCoord[0][1]);
			if (yp >= 0 && yp < src.rows&&xp >= 0 && xp < src.cols) {
				dst.at<uchar>(y, x) = src.at<uchar>(yp, xp);
			}
		}
	}
}