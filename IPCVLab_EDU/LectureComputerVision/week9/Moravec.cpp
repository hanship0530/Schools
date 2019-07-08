#include "Moravec.h"
#include <algorithm> 
#include <queue>
#include <utility> 

Moravec::Moravec(const cv::Mat &src)
{
	c.create(src.size(), CV_32FC1);

	// 외곽 2개 픽셀의 Confidence는 0으로 가정
	for(int w = 0; w < src.cols; ++w)
	{
		c.at<float>(0, w) = 0.0;
		c.at<float>(1, w) = 0.0;
		c.at<float>(src.rows-1, w) = 0.0;
		c.at<float>(src.rows-2, w) = 0.0;
	}

	for(int h = 0; h < src.rows; ++h)
	{
		c.at<float>(h, 0) = 0.0;
		c.at<float>(h, 1) = 0.0;
		c.at<float>(h, src.cols-1) = 0.0;
		c.at<float>(h, src.cols-2) = 0.0;
	}
}


void Moravec::CreateC(const cv::Mat &src)
{
	//모라벡 알고리즘을 구현하시오.

	int y, x;
	for (y = 2; y < src.rows-2; y++) {
		for (x = 2; x < src.cols-2; x++) {
			std::queue<std::pair<int, int>> points;
			points.push(std::pair<int, int>(y-1, x));
			points.push(std::pair<int, int>(y, x+1));
			points.push(std::pair<int, int>(y, x-1));
			points.push(std::pair<int, int>(y+1, x));
			float S[4] = { 0, };
			int index = 0;
			while (points.size() != 0) {
				std::pair<int, int> temp = points.front();
				points.pop();
				int j = temp.first;
				int i = temp.second;
				for (int b = -1; b <= 1; b++) {
					for (int a = -1; a <= 1; a++) {
						S[index] += std::pow(src.at<uchar>(j + b, i + a) - src.at<uchar>(y + b, x + a), 2);
					}
				}
				index++;
			}
			float *min = std::min_element(S, S + 3);
			c.at<float>(y, x) = *min;
		}
	}
}

void Moravec::DrawFeature(const cv::Mat &dst)
{

	for(int h = 0; h < c.rows; ++h)
	{
		for(int w = 0; w < c.cols; ++w)
		{
			if(c.at<float>(h,w) >= 15000)
			{
				circle(dst, cv::Point(w,h), 3, cv::Scalar(255, 0, 0));
			}
		}
	}
}