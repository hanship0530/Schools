#include "LogFilter.h"
#include <cmath>
#include <iostream>
using namespace std;
Filter::~Filter()
{
	if(mask != nullptr)
	{
		for(int i = 0; i < maskSize; ++i)
		{
			delete []mask[i];
		}
		delete []mask;
	}
}

void Filter::AllocateMask()
{
	mask = new double*[maskSize];

	for(int i = 0; i < maskSize; ++i)
	{
		mask[i] = new double[maskSize];
	}

	for(int j = 0; j < maskSize; ++j)
	{
		for(int i = 0; i < maskSize; ++i)
		{
			mask[j][i] = 0;
		}
	}
}

void Filter::Convolute(const cv::Mat &org)
{
	logReult.create(org.size(), CV_32FC1);	
	int halfSize = maskSize/2;
	for(int y = halfSize; y < org.cols-halfSize; ++y)
	{
		for(int x = halfSize; x < org.rows-halfSize; ++x)
		{
			double sum = 0;
			for(int maskY = -halfSize; maskY <= halfSize; ++maskY)
			{
				for(int maskX = -halfSize; maskX <= halfSize; ++maskX)
				{
					sum += (org.at<uchar>(y + maskY, x + maskX) *
						mask[halfSize + maskY][halfSize + maskX]);
				}
			}
			logReult.at<float>(y, x) = sum;
		}
	}	
}

void LOG::CreateMask(double sigma)
{
	maskSize =  6 *sigma;
	if(maskSize % 2 == 0)
		++maskSize;

	AllocateMask();

	int halfSize = maskSize/2;

	double sigmaSquare =  2 * sigma * sigma;
	double sigma4thPow =  sigma * sigma * sigma * sigma;

	//G(y,x)
	for(int j = -halfSize; j <= halfSize; ++j)
	{
		for(int i = -halfSize; i <= halfSize; ++i)
		{
			double numerator = exp(-((j*j + i*i) / sigmaSquare));
			double denominator = 3.14 * sigmaSquare;

			mask[halfSize+j][halfSize+i] = numerator / denominator;
		}
	}

	//NABLA^2 G(y,x)를 만드시오.
	for(int j = -halfSize; j <= halfSize; ++j)
	{
		for(int i = -halfSize; i <= halfSize; ++i)
		{		
			int y = halfSize + j;
			int x = halfSize + i;
			mask[y][x] = mask[y][x] * ((j * j + i * i - sigmaSquare) / sigma4thPow);
		}
	}
	double sum=0;
	for (int j = -halfSize; j <= halfSize; ++j)
	{
		for (int i = -halfSize; i <= halfSize; ++i)
		{
			int y = halfSize + j;
			int x = halfSize + i;
			sum += mask[y][x];
			cout << mask[y][x] << " ";
		}
		cout << endl;
	}
	cout << sum << endl;
}

int LOG::CheckSign(float val1, float val2)
{
	// 기호에 따라 값을 다르게 하시오
	if (val1 > 0.0 && val2 < 0.0) {
		return 1;
	}
	else if (val1 < 0.0 && val2 > 0.0) {
		return -1;
	}
	// 대신, 값이 0인 경우 0으로 리턴하게 하시오.
	return 0;
}

void LOG::FindZeroCrossing(cv::Mat &out)
{
	for(int h = 1; h < logReult.cols-1; ++h)
	{
		for(int w = 1; w < logReult.rows-1; ++w)
		{
			int cnt = 0;
			float n0 = logReult.at<float>(h, w+1);
			// n1~n7까지 적절한 값을 넣으시오.
			float n4 = logReult.at<float>(h, w - 1);
			float n1 = logReult.at<float>(h + 1, w + 1);
			float n5 = logReult.at<float>(h - 1, w - 1);
			float n2 = logReult.at<float>(h - 1, w + 1);
			float n6 = logReult.at<float>(h + 1);
			float n3 = logReult.at<float>(h + 1, w - 1);
			float n7 = logReult.at<float>(h - 1, w + 1);
			//CheckSign 함수를 이용하여 영교차 알고리즘을 적용하시오.
			if (CheckSign(n0, n4) != 0) {
				cnt++;
			}
			if (CheckSign(n1, n5) != 0) {
				cnt++;
			}
			if (CheckSign(n2, n6) != 0) {
				cnt++;
			}
			if (CheckSign(n3, n7) != 0) {
				cnt++;
			}
			if(cnt >= 2)
				out.at<uchar>(h,w) = 255;
		}
	}
}