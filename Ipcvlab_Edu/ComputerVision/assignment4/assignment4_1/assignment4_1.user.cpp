#include <iostream>
#include <queue>
#include <tuple>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace IPCVL {
	namespace UTIL {
		// 가우시안 함수
		double gaussian(int y, int x, double stdDev) {
			return (1.0 / (2 * M_PI * pow(stdDev, 2)))
				* pow(M_E, -((pow(y, 2) + pow(x, 2)) / (2 * pow(stdDev, 2))));
		}

		// 두 변수의 부호 비교 함수
		bool compareDifferentSign(double a, double b) {
			return !((a <= 0 && b <= 0) || (a > 0 && b > 0));
		}

		// 필터 내용 표시 함수
		// 필터를 콘솔에서 직접 확인해볼 수 있다.
		void displayFilter(cv::Mat filter) {
			using namespace std;

			cout << filter.rows << "x" << filter.cols << " Filter Mat[\n";
			for (int j = 0; j < filter.rows; j++) {
				for (int i = 0; i < filter.cols; i++) {
					printf("%.3f", filter.at<double>(j, i));
					if (i == filter.cols - 1)
						cout << endl;
					else
						cout << ",\t";
				}
				if (j == filter.rows - 1)
					cout << "]\n\n";
			}
		}
	} // namespace UTIL

	namespace IMG_PROC {
		// 영교차를 찾아 이미지 이진화를 진행하는 함수 (영교차 이진화 함수)
		// Hint 1: 영교차 검출 방법은 pp. 127 아래의 <현실적인 두 가지 규칙>을 따라 사용합니다.
		//         threshold는 그 중 임계값이며, UTIL::compareDifferentSign을 활용해 보세요.
		// Hint 2: 이진화는 0.0과 255.0로 진행합니다.
		void calculateZeroCrossingBinaryImage(cv::InputArray inputImage, cv::OutputArray outputImage, double threshold=0.0) {

			/* Your code here! */
			//outputImage.create(inputImage.size(), CV_64FC1);
			cv::Mat srcMat = inputImage.getMat();
			cv::Mat dstMat = cv::Mat::zeros(srcMat.size(), srcMat.type());
			outputImage.assign(dstMat);

			for (int y = 1; y < srcMat.rows - 1; y++) {
				for (int x = 1; x < srcMat.cols - 1; x++) {
					int zeroCrossingCount = 0;
					if (IPCVL::UTIL::compareDifferentSign(srcMat.at<double>(y - 1, x - 1), srcMat.at<double>(y + 1, x + 1))) {
						if (abs(srcMat.at<double>(y - 1, x - 1) - srcMat.at<double>(y + 1, x + 1)) >= threshold) {
							zeroCrossingCount++;
						}
					}
					if (IPCVL::UTIL::compareDifferentSign(srcMat.at<double>(y - 1, x), srcMat.at<double>(y + 1, x))) {
						if (abs(srcMat.at<double>(y - 1, x) - srcMat.at<double>(y + 1, x)) >= threshold) {
							zeroCrossingCount++;
						}
					}
					if (IPCVL::UTIL::compareDifferentSign(srcMat.at<double>(y - 1, x + 1), srcMat.at<double>(y + 1, x - 1))) {
						if(abs(srcMat.at<double>(y - 1, x + 1) - srcMat.at<double>(y + 1, x - 1)) >= threshold) {
							zeroCrossingCount++;
						}
					}
					if (IPCVL::UTIL::compareDifferentSign(srcMat.at<double>(y, x - 1), srcMat.at<double>(y, x + 1))) {
						if (abs(srcMat.at<double>(y, x - 1) - srcMat.at<double>(y, x + 1)) >= threshold) {
							zeroCrossingCount++;
						}
					}
					if (zeroCrossingCount >= 2) {
						dstMat.at<double>(y, x) = 255.0;
					}
				}
			}
		}

		// 가우시안 스무싱 필터를 만드는 함수
		// 입력으로 주어진 가우시안 표준편차를 이용합니다.
		// 필터는 항상 홀수 크기로 만들어진다고 가정합니다 (교재 내용 참조)
		// Hint 1: 가우시안 필터의 크기는 pp. 123의 <대략적인 규칙>을 따라 사용합니다.
		void calculateGaussianSmoothingFilter(double gaussianStdDev, cv::OutputArray gaussianFilter) {

			/* Your code here! */
			int filterSize = gaussianStdDev*6.0;
			if (filterSize % 2 == 0) {
				filterSize++;
			}
			cv::Mat gaussianMat = cv::Mat::zeros(filterSize, filterSize, CV_64FC1);
			gaussianFilter.assign(gaussianMat);

			for (int y = 0; y < filterSize; y++) {
				for (int x = 0; x < filterSize; x++) {
					gaussianMat.at<double>(y, x) = IPCVL::UTIL::gaussian(y-filterSize/2, x-filterSize/2, gaussianStdDev);
				}
			}
		}

		// Marr-Hildreth 에지 검출을 수행합니다.
		void doMarrHildrethEdgeDetection(cv::InputArray inputImage, cv::OutputArray outputImage, double gaussianStdDev, double threshold) {
			outputImage.create(inputImage.size(), inputImage.type());

			// 1. `gaussianStdDev` 크기의 가우시안 필터를 만들어, inputImage에 스무딩(컨볼루션)을 진행한다.
			// Hint 1: 가우시안 필터의 크기는 pp. 123의 <대략적인 규칙>을 따라 사용합니다.
			// Hint 2: cv::filter2D(input, output, -1, filter)를 사용하면 컨볼루션을 진행할 수 있습니다.
			cv::Mat smoothFilter, gaussianFilterOutput;
			calculateGaussianSmoothingFilter(gaussianStdDev, smoothFilter);
			cv::filter2D(inputImage, gaussianFilterOutput, -1, smoothFilter);
			UTIL::displayFilter(smoothFilter);

			// 2. 라플라시안 연산자를 이용하여 2차 미분을 구한다.
			// Hint: 라플라시안 행렬을 pp. 125를 참고하여 만들고, 1번 문제와 동일하게 컨볼루션 진행합니다.
			double _laplacianData[3][3]{ {0., 1., 0.}, {1., -4., 1.}, {0., 1., 0.} };
			cv::Mat laplacian(3, 3, CV_64FC1, _laplacianData), laplacianOutput;
			cv::filter2D(gaussianFilterOutput, laplacianOutput, -1, laplacian);

			// 3. 결과 영상에서 영교차를 찾아 에지(255)로 설정하고, 나머지는 비에지(0)로 설정한다.
			// Hint: 영교차 검출 방법은 pp. 127 아래의 <현실적인 두 가지 규칙>을 따라 사용합니다.
			calculateZeroCrossingBinaryImage(laplacianOutput, outputImage, threshold);
		}
	}  // namespace IMG_PROC
}

using namespace std;

int main() {
	// Read input image
	// 이번 과제는 CV_64FC1으로 Type을 통일합니다.
	// 문제는 CV_64FC1으로 풀며, 결과는 출력 문제가 발생하므로 main문에서 CV_8UC1으로 변환 출력합니다.
	cv::Mat inputImage = cv::imread("input.bmp" , cv::IMREAD_GRAYSCALE); // 반드시 채점시에는 input.bmp로 변경해주세요.
	inputImage.convertTo(inputImage, CV_64FC1); // 변환된 형 재확인. 
	cv::Mat outputImageA, outputImageB;

	// Marr-Hildreth 에지 검출을 수행합니다.
	IPCVL::IMG_PROC::doMarrHildrethEdgeDetection(inputImage, outputImageA, 2.0, 4.5); // 제출시 2.0, 4.5
	IPCVL::IMG_PROC::doMarrHildrethEdgeDetection(inputImage, outputImageB, 1.0, 6.0); // 제출시 1.0, 6.0

	inputImage.convertTo(inputImage, CV_8UC1);
	outputImageA.convertTo(outputImageA, CV_8UC1);
	outputImageB.convertTo(outputImageB, CV_8UC1);

	cv::imwrite("user_out_2_45.bmp", outputImageA); // 반드시 채점 돌리기 전 파일이 잘 출력되었는지 확인해주세요.
	cv::imwrite("user_out_1_6.bmp", outputImageB);

	// 아래 코드는 채점시에는 작동되지 않습니다.
#ifndef _JUDGE

	// 결과 확인 코드
	cv::namedWindow("Source Image", cv::WINDOW_NORMAL);
	cv::namedWindow("MarrHildreth Edge Detection Image (2.0=>4.5)", cv::WINDOW_NORMAL);
	cv::namedWindow("MarrHildreth Edge Detection Image (1.0=>6.0)", cv::WINDOW_NORMAL);

	cv::resizeWindow("Source Image", 600, 800);
	cv::resizeWindow("MarrHildreth Edge Detection Image (2.0=>4.5)", 600, 800);
	cv::resizeWindow("MarrHildreth Edge Detection Image (1.0=>6.0)", 600, 800);

	cv::imshow("Source Image", inputImage);
	cv::imshow("MarrHildreth Edge Detection Image (2.0=>4.5)", outputImageA);
	cv::imshow("MarrHildreth Edge Detection Image (1.0=>6.0)", outputImageB);
	cv::waitKey();
	// 결과 확인 코드 끝

#endif

	return 0;
}