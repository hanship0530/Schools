#include "Filter.h"
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

cv::Mat Filter::Convolute(const cv::Mat &org)
{
	cv::Mat out(org.size(), CV_8UC1);
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
			out.at<uchar>(y, x) = cv::saturate_cast<uchar>(sum);
		}
	}

	return out;
}


void SobelFilterY::CreateMask()
{
	maskSize = 3;

	AllocateMask();

	//sobelY Filter를 구현하시오.
	mask[0][0] = -1;
	mask[0][1] = -2;
	mask[0][2] = -1;

	mask[1][0] = 0;
	mask[1][1] = 0;
	mask[2][2] = 0;

	mask[2][0] = 1;
	mask[2][1] = 2;
	mask[2][2] = 1;
}

void SobelFilterX::CreateMask()
{
	maskSize = 3;

	AllocateMask();

	//sobelX Filter를 구현하시오.
	mask[0][0] = -1;
	mask[0][1] = 0;
	mask[0][2] = 1;

	mask[1][0] = -2;
	mask[1][1] = 0;
	mask[1][2] = 2;

	mask[2][0] = -1;
	mask[2][1] = 0;
	mask[2][2] = 1;
}