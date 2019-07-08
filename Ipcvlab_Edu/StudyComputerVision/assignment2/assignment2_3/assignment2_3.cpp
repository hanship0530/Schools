#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace IPCVL {
	namespace IMG_PROC {
		enum CONNECTIVITIES {
			NAIVE_FOURWAY = 0,
			NAIVE_EIGHT_WAY
		};

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
		// 4방향 flood_fill 알고리즘
		void flood_fill4(cv::Mat & l, const int & j, const int & i, const int & label) {
			// Your code here!
			// Hint: Boundary를 감안해서 풀면 됩니다.
			if (l.at<int>(j, i) == -1) {
				l.at<int>(j, i) = label;
				flood_fill4(l, j, i + 1, label);
				flood_fill4(l, j - 1, i, label);
				flood_fill4(l, j, i - 1, label);
				flood_fill4(l, j + 1, i, label);
			}
		}

		// 8방향 flood_fill 알고리즘
		void flood_fill8(cv::Mat & l, const int & j, const int & i, const int & label) {
			// Your code here!
			// Hint: Boundary를 감안해서 풀면 됩니다.
			if (l.at<int>(j, i) == -1) {
				l.at<int>(j, i) = label;
				flood_fill8(l, j - 1, i - 1, label);
				flood_fill8(l, j - 1, i, label);
				flood_fill8(l, j - 1, i + 1, label);
				flood_fill8(l, j, i - 1, label);
				flood_fill8(l, j, i + 1, label);
				flood_fill8(l, j + 1, i - 1, label);
				flood_fill8(l, j + 1, i, label);
				flood_fill8(l, j + 1, i + 1, label);
			}
		}

		// flood_fill 알고리즘 껍데기
		void flood_fill(cv::InputArray src, cv::OutputArray dst, const CONNECTIVITIES & direction) {
			// Your code here!
			// Hint1: CONNECTIVITIES에 따라서 어떤 알고리즘을 쓸 것인가가 결정됩니다.
			// Hint2: 영상 이진화를 먼저 수행하면 간헐적으로 결과 이미지가 깨지는 문제를 회피할 수 있습니다.
			//        (입력 이미지 손실압축으로 인해 입력값이 0이 아닌 1, 255가 아닌 254일 수도 있습니다.)
			dst.create(src.size(), CV_32SC1);
			cv::Mat srcMat = src.getMat();
			cv::Mat dstMat = dst.getMat();

			for (int y = 0; y < srcMat.rows; y++) {
				for (int x = 0; x < srcMat.cols; x++) {
					if (srcMat.at<uchar>(y, x) == 255) {
						dstMat.at<int>(y, x) = -1;
						continue;
					}
					dstMat.at<int>(y, x) = 0;
				}
			}

			for (int y = 0; y < dstMat.rows; y++) {
				dstMat.at<int>(y, 0) = 0;
				dstMat.at<int>(y, dstMat.cols - 1) = 0;
			}

			for (int x = 0; x < dstMat.cols; x++) {
				dstMat.at<int>(0, x) = 0;
				dstMat.at<int>(dstMat.rows - 1, x) = 0;
			}

			if (direction == 0) {
				int label = 1;
				for (int y = 1; y < dstMat.rows - 1; y++) {
					for (int x = 1; x < dstMat.cols - 1; x++) {
						if (dstMat.at<int>(y, x) == -1) {
							flood_fill4(dstMat, y, x, label);
							label++;
						}
					}
				}
			}
			if (direction == 1) {
				int label = 1;
				for (int y = 1; y < dstMat.rows - 1; y++) {
					for (int x = 1; x < dstMat.cols - 1; x++) {
						if (dstMat.at<int>(y, x) == -1) {
							flood_fill8(dstMat, y, x, label);
							label++;
						}
					}
				}
			}
		}
	}  // namespace IMG_PROC
}

using namespace std;

int main() {
	string inputFilename;
	cin >> inputFilename;

	// Read input image
	cv::Mat inputImage = cv::imread(inputFilename, cv::IMREAD_GRAYSCALE);
	cv::Mat result_naive_4, result_naive_8;

	// Manipulate image with floodfill algorithm
	IPCVL::IMG_PROC::flood_fill(inputImage, result_naive_4, IPCVL::IMG_PROC::CONNECTIVITIES::NAIVE_FOURWAY);
	IPCVL::IMG_PROC::flood_fill(inputImage, result_naive_8, IPCVL::IMG_PROC::CONNECTIVITIES::NAIVE_EIGHT_WAY);
	
	double min, max;

	cv::minMaxLoc(result_naive_4, &min, &max);
	result_naive_4.convertTo(result_naive_4, CV_8UC1, 255 / max);

	cv::minMaxLoc(result_naive_8, &min, &max);
	result_naive_8.convertTo(result_naive_8, CV_8UC1, 255 / max);


	// 아래 코드는 채점시에는 작동되지 않습니다.

#ifndef _JUDGE
	//Show Result
	cv::namedWindow("src Image", cv::WINDOW_NORMAL);
	cv::namedWindow("floodfill naive 4-way", cv::WINDOW_NORMAL);
	cv::namedWindow("floodfill naive 8-way", cv::WINDOW_NORMAL);

	cv::resizeWindow("src Image", 640, 640);
	cv::resizeWindow("floodfill naive 4-way", 640, 640);
	cv::resizeWindow("floodfill naive 8-way", 640, 640);

	cv::imshow("src Image", inputImage);
	cv::imshow("floodfill naive 4-way", result_naive_4);
	cv::imshow("floodfill naive 8-way", result_naive_8);
	cv::waitKey();
#endif

	cv::imwrite("output_native_4.jpg", result_naive_4);
	cv::imwrite("output_native_8.jpg", result_naive_8);

	return 0;
}