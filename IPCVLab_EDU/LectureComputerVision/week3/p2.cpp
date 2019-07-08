#include "opencv2\opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void MakeHistogram(Mat &src, float hist[256])
{
	for(int y=0; y<src.rows; y++){
		for(int x=0; x<src.cols; x++){
			hist[src.at<uchar>(y,x)]++;
		}
	}
}


void ShowHistogram(float hist[256])
{

	float cpHist[256];
	memcpy(cpHist, hist, sizeof(float) * 256);
	Mat histMat = Mat(256, 1, CV_32FC1, cpHist);

	int histSize = 256;

	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

	normalize(histMat, histMat, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	// Draw for each channel
	for( int i = 1; i < histSize; i++ )
	{
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(histMat.at<float>(i-1)) ) ,
			Point( bin_w*(i), hist_h - cvRound(histMat.at<float>(i)) ),
			Scalar( 255, 0, 255), 2, 8, 0  );
	}

	/// Display
	namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
	imshow("calcHist Demo", histImage );

	waitKey(0);
}

int main()
{
	Mat src, gray;

	/// Load image
	src = imread("lena_color.jpg",CV_LOAD_IMAGE_COLOR);
	gray = imread("lena_color.jpg",CV_LOAD_IMAGE_GRAYSCALE);

	if(src.empty()){
		cout << "파일이 없습니다. " << endl;
		return -1;
	}

	Mat YCbCrMat;
	std::vector<Mat> channels; //split 함수를 사용할때 분리된 채널을 넣어줄 변수
	//YCrCb로 컨버팅 후에 Y채널만 이용하여 히스토그램을 만들고 GrayScale과 비교해보시오.
	Mat YCrCbMat, SplitedMat[3];
	cvtColor(src, YCbCrMat, CV_BGR2YCrCb);
	split(YCbCrMat, SplitedMat);

	//GrayScale 히스토그램
	float histArrG[256] = {0.0, };
	float histArrY[256] = {0.0, };
	MakeHistogram(gray, histArrG);
	//MakeHistogram(SplitedMat[0], histArrY);
	ShowHistogram(histArrG);
	//ShowHistogram(histArrY);

	return 0;
}