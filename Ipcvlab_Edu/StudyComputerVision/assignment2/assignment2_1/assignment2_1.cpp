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
	}  // namespace IMG_PROC
}

using namespace std;

int main() {
	string inputFilename, outputFilename;
	cin >> inputFilename >> outputFilename;
	// Read input image
	cv::Mat inputImage = cv::imread(inputFilename, cv::IMREAD_GRAYSCALE);
	cv::Mat userResult, opencvResult;
	const int THRESHOLD = 125;

	IPCVL::IMG_PROC::thresh_binary(inputImage, userResult, THRESHOLD);
	cv::threshold(inputImage, opencvResult, THRESHOLD, 255, cv::THRESH_BINARY);

	// 아래 코드는 채점시에는 작동되지 않습니다.
	// Opencv Result와 내가 짠 Our Result의 값을 비교해볼 수 있습니다.

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