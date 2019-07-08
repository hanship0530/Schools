#include "LineFitting.h"
#include <iostream>
using namespace std;

void CRANSAC::DrawLine(cv::Mat &out)
{
	 for(int i=0; i<out.cols; i++) //직선의 방정식을 영상에 출력
    {
		double sum3 = (opM*i) + opB;
		if(sum3 < 0)
			sum3 = 0;
		if(sum3 > out.cols -1)
			sum3 = out.cols -1;
        out.at<uchar>(sum3, i) = 255; 
 
    }
}

void CRANSAC::DrawPoint(cv::Mat &out)
{
	for(int i = 0; i < ptVec.size(); ++i)
		out.at<uchar>(ptVec[i].y, ptVec[i].x) = 255;
}

void CRANSAC::CalLine()
{
	// Point pt1과 Point pt2를 이용해서 기울기와 절편을 계산하세요
	m = (pt2.y - pt1.y) / (pt2.x - pt1.x);
	b = pt1.y - m * pt1.x;
}

void CRANSAC::RandomGetPT()
{
	pt1Idx =getRandomNumber(0, ptVec.size()-1);
	pt1 = ptVec[pt1Idx];
	
	pt2Idx =getRandomNumber(0, ptVec.size()-1);
	pt2 = ptVec[pt2Idx];	
	while(pt1 == pt2 || pt1.x == pt2.x || pt1.y == pt2.y)
	{
		pt2Idx =getRandomNumber(0, ptVec.size()-1);
		pt2 = ptVec[pt2Idx];
	}	
}

void CRANSAC::MakeRandomPoint()
{
	for ( int i = 0 ; i < 20; i++ )
    {
        int x = getRandomNumber(0, 280);
		int y = getRandomNumber(120, 125);

		ptVec.push_back(cv::Point(x,y));		
    }	
	
	// outlier
	for ( int i = 0 ; i < 20; i++ )
    {
        int x = getRandomNumber(0, 280);
		int y = getRandomNumber(0, 280);

		ptVec.push_back(cv::Point(x,y));		
    }	
}

void CRANSAC::CalInliner()
{
	std::vector<cv::Point> liner;
	for ( int i = 0 ; i < ptVec.size(); i++ )
	{
		if(i != pt1Idx && i != pt2Idx)
		{
			// distance = |(-(x * m) - b + y) / root(m^2 + 1)|
			// ptVec[i]를 이용해서 거리를 계산하세요
			double distance = abs((-(ptVec.at(i).x * m) - b + ptVec.at(i).y) / sqrt(pow(m,2) + 1));
			if (distance <= 5) {
				//cout << distance << endl;
				liner.push_back(ptVec[i]);
			}
		}
	}
	if(maxInlier < liner.size())
	{
		maxInlier = liner.size();
		opM = m;
		opB = b;
	}
}

void LeastSquare::DrawLine(cv::Mat &out)
{
	 for(int i=0; i<out.cols; i++) //직선의 방정식을 영상에 출력
    {
		double sum3 = (m*i) + b;
		if(sum3 < 0)
			sum3 = 0;
		if(sum3 > out.cols -1)
			sum3 = out.cols -1;
        out.at<uchar>(sum3, i) = 255; 
 
    }
}

void LeastSquare::DrawPoint(cv::Mat &out)
{
	for(int i = 0; i < ptVec.size(); ++i)
		out.at<uchar>(ptVec[i].y, ptVec[i].x) = 255;
}

void LeastSquare::CalLine()
{
	double sumX = 0.0;
	double sumY = 0.0;

	double avgX = 0.0;
	double avgY = 0.0;

	//기울기와 절편을 구하시오.
	for(int i = 0; i < ptVec.size(); i++)
	{
		sumX += ptVec.at(i).x;
		sumY += ptVec.at(i).y;
	}
	
	avgX = sumX / ptVec.size();
	avgY = sumY / ptVec.size();

	double sumNumerator = 0.0;
	double sumDenominator = 0.0;

	for(int i = 0; i < ptVec.size(); i++)
	{
		sumNumerator += (ptVec.at(i).y - avgY)*(ptVec.at(i).x - avgX);
		sumDenominator += pow(ptVec.at(i).x - avgX, 2);
	}

	m = sumNumerator / sumDenominator;
	b = avgY - m * avgX;
}

void LeastSquare::MakeRandomPoint()
{
	for ( int i = 0 ; i < 20; i++ )
    {
        int x = getRandomNumber(0, 280);
		int y = getRandomNumber(120, 125);

		ptVec.push_back(cv::Point(x,y));		
    }	
	
	// outlier
	for ( int i = 0 ; i < 20; i++ )
    {
        int x = getRandomNumber(0, 280);
		int y = getRandomNumber(0, 280);

		ptVec.push_back(cv::Point(x,y));		
    }	
}