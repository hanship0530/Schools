#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace IPCVL {
	namespace IMG_PROC {

		// 영상 이진화 알고리즘 (T=threshold)
		void thresh_binary(cv::InputArray src, cv::OutputArray dst, const int & threshold) {
			// Your code Here!
			cv::Mat srcMat = src.getMat();
			dst.create(src.size(), CV_8UC1);
			cv::Mat dstMat = dst.getMat();
			for (int y = 0; y < srcMat.rows; y++) {
				for (int x = 0; x < srcMat.cols; x++) {
					if (srcMat.at<uchar>(y, x) > threshold) {
						dstMat.at<uchar>(y, x) = 255;
						continue;
					}
					dstMat.at<uchar>(y, x) = 0;
				}
			}
		}

		// 효율적인 오츄 알고리즘
		void thresh_otsu(cv::InputArray src, cv::OutputArray dst) {
			// Your code here!
			// Hint: 주어진 문제의 알고리즘과 (식 2.6~2.9)를 참고하여 구현하면 됩니다.
			dst.create(src.size(), CV_8UC1);
			cv::Mat srcMat = src.getMat();
			cv::Mat dstMat = dst.getMat();

			double hatHistogram[256] = { 0.0, };
			double weightZero[256];
			double meanZero[256];
			double meanOne[256];
			double varianceBetween[256];
			double mean = 0;
			double max = 0.0;
			int threshold = 0;
			int L = 256;
			for (int y = 0; y < srcMat.rows; y++) {
				for (int x = 0; x < srcMat.cols; x++) {
					// Todo : histogram을 쌓습니다.

					hatHistogram[srcMat.at<uchar>(y, x)]++;
					// hint 1 : for loop 를 이용해서 cv::Mat 순회 시 (1채널의 경우) 
					// inputMat.at<uchar>(y, x)와 같이 데이터에 접근할 수 있습니다. 
				}
			}

			weightZero[0] = hatHistogram[0];
			meanZero[0] = 0.0;
			varianceBetween[0] = 0.0;
			for (int i = 0; i < L; i++) {
				mean += i * hatHistogram[i];
			}

			for (int t = 1; t < L; t++) {
				weightZero[t] = weightZero[t - 1] + hatHistogram[t];
				if (weightZero[t] == 0.0 || (1 - weightZero[t]) == 0.0)
					continue;
				meanZero[t] = (weightZero[t - 1] * meanZero[t - 1] + t * hatHistogram[t]) / weightZero[t];
				meanOne[t] = (mean - weightZero[t] * meanZero[t]) / (1 - weightZero[t]);
				varianceBetween[t] = weightZero[t] * (1 - weightZero[t])*(meanZero[t] - meanOne[t])*(meanZero[t] - meanOne[t]);
			}

			max = varianceBetween[1];

			for (int i = 2; i < L; i++) {
				if (max < varianceBetween[i]) {
					max = varianceBetween[i];
					threshold = i;
				}
			}
			thresh_binary(srcMat, dstMat, threshold);
		}
	}  // namespace IMG_PROC
}

using namespace std;

int main() {
	string inputFilename, outputFilename;
	cin >> inputFilename >> outputFilename;

	// Read input image
	cv::Mat inputImage = cv::imread(inputFilename, cv::IMREAD_GRAYSCALE);
	cv::Mat userResult, opencvResult;

	IPCVL::IMG_PROC::thresh_otsu(inputImage, userResult);
	cv::threshold(inputImage, opencvResult, 0, 255, cv::THRESH_OTSU);

	// 아래 코드는 채점시에는 작동되지 않습니다.

#ifndef _JUDGE
	//Show Result
	cv::imshow("Input Image", inputImage);
	cv::imshow("Opencv Result", opencvResult);
	cv::imshow("Our Result", userResult);
	cv::waitKey();
#endif

	cv::imwrite(outputFilename, userResult);

	return 0;
}