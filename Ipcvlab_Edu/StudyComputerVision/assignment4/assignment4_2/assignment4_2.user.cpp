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
			return (1.0 / (2.0 * M_PI * pow(stdDev, 2)))
				* pow(M_E, -1.0 * ((pow(y, 2) + pow(x, 2)) / (2 * pow(stdDev, 2))));
		}

		// 두 변수의 부호 비교 함수
		bool compareDifferentSign(double a, double b) {
			return !((a <= 0 && b <= 0) || (a > 0 && b > 0));
		}

		// LOG 함수
		// Hint: 위 gaussian() 함수를 활용한다.
		double logFilter(int j, int i, int stdDev) {

			/* Your code here! */
			return ((pow(j,2)+pow(i,2)-2*pow(stdDev,2))/pow(stdDev,4))*gaussian(j,i,stdDev);
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
		void calculateZeroCrossingBinaryImage(cv::InputArray inputImage, cv::OutputArray outputImage, double threshold = 0.0) {
			cv::Mat inputMat = inputImage.getMat();
			cv::Mat outputMat = cv::Mat::zeros(inputMat.size(), inputMat.type());
			outputImage.assign(outputMat); // TODO: Check working

			for (int j = 1; j < inputMat.rows - 1; j++)
				for (int i = 1; i < inputMat.cols - 1; i++) {
					int zeroCrossDifferenceCount = 0;
					int zeroCrossThresholdCount = 0;

					if (UTIL::compareDifferentSign(inputMat.at<double>(j - 1, i), inputMat.at<double>(j + 1, i))) {
						zeroCrossDifferenceCount++;
						zeroCrossThresholdCount += (abs(inputMat.at<double>(j - 1, i) - inputMat.at<double>(j + 1, i)) >= threshold) ? 1 : 0;
					}

					if (UTIL::compareDifferentSign(inputMat.at<double>(j, i + 1), inputMat.at<double>(j, i - 1))) {
						zeroCrossDifferenceCount++;
						zeroCrossThresholdCount += (abs(inputMat.at<double>(j, i + 1) - inputMat.at<double>(j, i - 1)) >= threshold) ? 1 : 0;
					}

					if (UTIL::compareDifferentSign(inputMat.at<double>(j - 1, i - 1), inputMat.at<double>(j + 1, i + 1))) {
						zeroCrossDifferenceCount++;
						zeroCrossThresholdCount += (abs(inputMat.at<double>(j - 1, i - 1) - inputMat.at<double>(j + 1, i + 1)) >= threshold) ? 1 : 0;
					}

					if (UTIL::compareDifferentSign(inputMat.at<double>(j - 1, i + 1), inputMat.at<double>(j + 1, i - 1))) {
						zeroCrossDifferenceCount++;
						zeroCrossThresholdCount += (abs(inputMat.at<double>(j - 1, i + 1) - inputMat.at<double>(j + 1, i - 1)) >= threshold) ? 1 : 0;
					}

					if (zeroCrossDifferenceCount >= 2 && zeroCrossThresholdCount >= 2)
						outputMat.at<double>(j, i) = 255.0;
				}
		}

		// Laplacian of Gaussian 필터를 구하는 함수
		// LoG (Laplacian of Gaussian) 필터는 Laplacian 필터로 이미지를 컨볼루션하고,
		// 그 뒤에 결과를 다시 입력 영상과 컨볼루션한다.
		// Hint 1: 1번문제에서 가우시안 필터를 생성할 때처럼, <대략적인 규칙>을 따라 필터의 크기를 정합니다.
		// Hint 2: 좌표계는 가운데가 0,0 이지만 필터는 좌측 상단이 0,0입니다.
		void getLOGFilter(cv::InputArray inputImage, cv::OutputArray filterImage, double gaussianStdDev) {

			/* Your code here! */
			int filterSize = gaussianStdDev*6.0;
			if(filterSize % 2==0) {
				filterSize++;
			}
			cv::Mat gaussianMat = cv::Mat::zeros(filterSize, filterSize, CV_64FC1);
			filterImage.assign(gaussianMat);

			for(int j=0; j<filterSize;j++){
				for(int i=0;i<filterSize;i++){
					gaussianMat.at<double>(j,i) = IPCVL::UTIL::logFilter(j-filterSize/2,i-filterSize/2,gaussianStdDev);
				}
			}
		}

		// LOG (Laplacian of Gaussian) 함수를 이용해 에지 검출을 수행합니다.
		// Hint: pp 125.에 따라 진행하면 됩니다.
		void doLOGFilteredEdgeDetection(cv::InputArray inputImage, cv::OutputArray outputImage, double gaussianStdDev, double threshold) {

			/* Your code here! */
			cv::Mat logFilter, logFilterOutput;
			getLOGFilter(inputImage, logFilter, gaussianStdDev);
			cv::filter2D(inputImage, logFilterOutput, -1, logFilter);

			calculateZeroCrossingBinaryImage(logFilterOutput, outputImage, threshold);
		}
	}  // namespace IMG_PROC
}

using namespace std;

int main() {
	// Read input image
	// 이번 과제는 CV_64FC1으로 Type을 통일합니다.
	// 문제는 CV_64FC1으로 풀며, 결과는 출력 문제가 발생하므로 main문에서 CV_8UC1으로 변환 출력합니다.
	cv::Mat inputImage = cv::imread("input.bmp", cv::IMREAD_GRAYSCALE); // 반드시 채점시에는 input.bmp로 변경해주세요.
	inputImage.convertTo(inputImage, CV_64FC1); // 변환된 형 재확인. 
	cv::Mat outputImageA, outputImageB;

	// Marr-Hildreth 에지 검출을 수행합니다.
	IPCVL::IMG_PROC::doLOGFilteredEdgeDetection(inputImage, outputImageA, 2.0, 4.5); // 제출시 2.0, 4.5
	IPCVL::IMG_PROC::doLOGFilteredEdgeDetection(inputImage, outputImageB, 1.0, 6.0); // 제출시 1.0, 6.0

	inputImage.convertTo(inputImage, CV_8UC1);
	outputImageA.convertTo(outputImageA, CV_8UC1);
	outputImageB.convertTo(outputImageB, CV_8UC1);

	cv::imwrite("user_out_2_45.bmp", outputImageA); // 반드시 채점 돌리기 전 파일이 잘 출력되었는지 확인해주세요.
	cv::imwrite("user_out_1_6.bmp", outputImageB);

	// 아래 코드는 채점시에는 작동되지 않습니다.
#ifndef _JUDGE

	// 결과 확인 코드
	cv::namedWindow("Source Image", cv::WINDOW_NORMAL);
	cv::namedWindow("LOG Edge Detection Image (2.0=>4.5)", cv::WINDOW_NORMAL);
	cv::namedWindow("LOG Edge Detection Image (1.0=>6.0)", cv::WINDOW_NORMAL);

	cv::resizeWindow("Source Image", 600, 800);
	cv::resizeWindow("LOG Edge Detection Image (2.0=>4.5)", 600, 800);
	cv::resizeWindow("LOG Edge Detection Image (1.0=>6.0)", 600, 800);

	cv::imshow("Source Image", inputImage);
	cv::imshow("LOG Edge Detection Image (2.0=>4.5)", outputImageA);
	cv::imshow("LOG Edge Detection Image (1.0=>6.0)", outputImageB);
	cv::waitKey();
	// 결과 확인 코드 끝

#endif

	return 0;
}