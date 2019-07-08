#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <queue>

namespace IPCVL {
	namespace IMG_PROC {

		// 효율적인 4방향 flood_fill 알고리즘
		void efficient_flood_fill4(cv::Mat & l, const int & j, const int & i, const int & label) {
			// Your code here!
			// Hint: C++에서 사용할수 있는 std::queue 또는 std::vector를 활용합니다.
			std::queue<int> Q;
			Q.push(j);
			Q.push(i);

			while (!Q.empty()) {
				int y = Q.front();
				Q.pop();
				int x = Q.front();
				Q.pop();
				if (l.at<int>(y, x) == -1) {
					int left, right;
					left = x;
					right = x;

					while (l.at<int>(y, left - 1) == -1) left--;
					while (l.at<int>(y, right + 1) == -1) right++;

					for (int c = left; c <= right; c++) {
						l.at<int>(y, c) = label;
						if (l.at<int>(y - 1, c) == -1 && (c == left || l.at<int>(y - 1, c - 1) != -1)) {
							Q.push(y - 1);
							Q.push(c);
						}
						if (l.at<int>(y + 1, c) == -1 && (c == left || l.at<int>(y + 1, c - 1) != -1)) {
							Q.push(y + 1);
							Q.push(c);
						}
					}
				}
			}
		}

		// flood_fill 알고리즘 껍데기
		void flood_fill(cv::InputArray src, cv::OutputArray dst) {
			// Your code here!
		
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
			int label = 1;
			for (int y = 1; y < srcMat.rows - 1; y++) {
				for (int x = 1; x < srcMat.cols - 1; x++) {
					if (dstMat.at<int>(y, x) == -1) {
						efficient_flood_fill4(dstMat, y, x, label);
						label++;
					}
				}
			}
		}
	}  // namespace IMG_PROC
}

using namespace std;

int main() {
	string inputFilename, outputFilename;
	cin >> inputFilename >> outputFilename;

	// Read input image
	cv::Mat inputImage = cv::imread(inputFilename, cv::IMREAD_GRAYSCALE);
	cv::Mat result_efficient_4;

	// Manipulate image with floodfill algorithm
	IPCVL::IMG_PROC::flood_fill(inputImage, result_efficient_4);
	
	double min, max;

	cv::minMaxLoc(result_efficient_4, &min, &max);
	result_efficient_4.convertTo(result_efficient_4, CV_8UC1, 255 / max);

	// 아래 코드는 채점시에는 작동되지 않습니다.

#ifndef _JUDGE
	//Show Result

	cv::namedWindow("src Image", cv::WINDOW_NORMAL);
	cv::namedWindow("efficient 4-way", cv::WINDOW_NORMAL);

	cv::resizeWindow("src Image", 640, 640);
	cv::resizeWindow("efficient 4-way", 640, 640);


	cv::imshow("src Image", inputImage);
	cv::imshow("efficient 4-way", result_efficient_4);
	cv::waitKey();
#endif

	cv::imwrite(outputFilename, result_efficient_4);

	return 0;
}