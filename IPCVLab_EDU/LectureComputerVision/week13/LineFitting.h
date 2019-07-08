#pragma once
#include "opencv2\opencv.hpp"
#include <random>
class CRANSAC
{
public:
	cv::Point pt1;	
	cv::Point pt2;
	int pt1Idx;
	int pt2Idx;
	
	double m; //기울기
	double b; //절편

	double opM; // 최적의 기울기
	double opB; // 최적의 절편
	std::vector<cv::Point> ptVec;   // 랜덤으로 생성된 점들을 저정한 벡터	

	int maxInlier;
public:
	CRANSAC(){maxInlier = 0;}
	void DrawLine(cv::Mat &out);
	void DrawPoint(cv::Mat &out);
	void CalLine();
	void RandomGetPT();
	void MakeRandomPoint();
	void CalInliner();

    // min ~ max 사이에서 균등한 확률 분포로 생성
    static int getRandomNumber ( int min, int max )
	{		
		std::random_device rn;
		std::mt19937_64 rnd( rn() );
	
		std::uniform_int_distribution<int> range(min, max);

		return range( rnd );
	}
};

class LeastSquare
{
	cv::Point pt1;	
	cv::Point pt2;
	
	double m; //기울기
	double b; //절편

	std::vector<cv::Point> ptVec;   // 랜덤으로 생성된 점들을 저정한 벡터	

public:
	LeastSquare(){}
	void DrawLine(cv::Mat &out);
	void DrawPoint(cv::Mat &out);
	void MakeRandomPoint();
	void CalLine();

    // min ~ max 사이에서 균등한 확률 분포로 생성
    static int getRandomNumber ( int min, int max )
	{		
		std::random_device rn;
		std::mt19937_64 rnd( rn() );
	
		std::uniform_int_distribution<int> range(min, max);

		return range( rnd );
	}
};