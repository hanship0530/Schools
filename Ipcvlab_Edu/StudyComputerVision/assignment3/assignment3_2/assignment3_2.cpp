#include <iostream>
#include <queue>
#include <tuple>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace IPCVL {
	namespace UTIL {
		void _createHeterogeneousArrayToMat(cv::OutputArray output, double matrix[][3]) {
			output.create(3, 3, CV_64FC1);
			cv::Mat outputMat = output.getMat();
			for (int j = 0; j < outputMat.rows; j++)
				for (int i = 0; i < outputMat.cols; i++)
					outputMat.at<double>(j, i) = matrix[j][i];
		}

		void createMoveTransMatrix(cv::OutputArray output, int dx, int dy) {
			double _matrix[3][3]{
				{1, 0, 0},
				{0, 1, 0},
				{dy, dx, 1}
			};

			_createHeterogeneousArrayToMat(output, _matrix);
		}

		void createRotateTransMatrix(cv::OutputArray output, double theta) {
			double sin_theta = sin(theta * CV_PI / 180);
			double cos_theta = cos(theta * CV_PI / 180);

			double _matrix[3][3]{
				{cos_theta, -sin_theta, 0},
				{sin_theta, cos_theta, 0},
				{0, 0, 1}
			};

			_createHeterogeneousArrayToMat(output, _matrix);
		}

		void createScaleTransMatrix(cv::OutputArray output, double dx, double dy) {
			double _matrix[3][3]{
				{dy, 0, 0},
				{0, dx, 0},
				{0, 0, 1}
			};

			_createHeterogeneousArrayToMat(output, _matrix);
		}

		void createTiltXTransMatrix(cv::OutputArray output, double dx) {
			double _matrix[3][3]{
				{1, dx, 0},
				{0, 1, 0},
				{0, 0, 1}
			};

			_createHeterogeneousArrayToMat(output, _matrix);
		}

		void createTiltYTransMatrix(cv::OutputArray output, double dy) {
			double _matrix[3][3]{
				{1, 0, 0},
				{dy, 1, 0},
				{0, 0, 1}
			};

			_createHeterogeneousArrayToMat(output, _matrix);
		}
	} // namespace UTIL

	namespace IMG_PROC {
		void forwardGeometricTransformation(cv::InputArray inputImage, cv::OutputArray outputImage, cv::Mat transformMatrix) {
			// Your code here!
			// 이전 assignment3_1에서의 코드
			outputImage.create(inputImage.size(), CV_8UC1);

			cv::Mat srcMat = inputImage.getMat();
			cv::Mat dstMat = outputImage.getMat();
			//dstMat.setTo(cv::Scalar(150.));
			for (int y = 0; y < srcMat.rows; y++) {
				for (int x = 0; x < srcMat.cols; x++) {
					cv::Mat axis(1, 3, CV_64FC1);
					axis.at<double>(0, 0) = y;
					axis.at<double>(0, 1) = x;
					axis.at<double>(0, 2) = 1;
					axis *= transformMatrix;
					if ((int)axis.at<double>(0, 0) < 0 || (int)axis.at<double>(0, 0) >= dstMat.rows
						|| (int)axis.at<double>(0, 1) < 0 || (int)axis.at<double>(0, 1) >= dstMat.cols) {
						continue;
					}
					dstMat.at<char>((int)axis.at<double>(0, 0), (int)axis.at<double>(0, 1)) = srcMat.at<char>(y, x);
				}
			}
		}

		void backwardGeometricTransformation(cv::InputArray inputImage, cv::OutputArray outputImage, cv::Mat transformMatrix) {
			// Your code here!
			// Hint 1: Inverse Matrix를 구하려면 <Mat 객체>.inv()를 이용한다.
			outputImage.create(inputImage.size(), CV_8UC1);

			cv::Mat srcMat = inputImage.getMat();
			cv::Mat dstMat = outputImage.getMat();
			//dstMat.setTo(cv::Scalar(128.));
			for (int y = 0; y < srcMat.rows; y++) {
				for (int x = 0; x < srcMat.cols; x++) {
					cv::Mat axis(1, 3, CV_64FC1);
					axis.at<double>(0, 0) = y;
					axis.at<double>(0, 1) = x;
					axis.at<double>(0, 2) = 1;
					axis *= transformMatrix.inv();
					if ((int)axis.at<double>(0, 0) < 0 || (int)axis.at<double>(0, 0) >= dstMat.rows
						|| (int)axis.at<double>(0, 1) < 0 || (int)axis.at<double>(0, 1) >= dstMat.cols) {
						continue;
					}
					dstMat.at<char>(y,x) = srcMat.at<char>((int)axis.at<double>(0, 0), (int)axis.at<double>(0, 1));
				}
			}
		}
	}  // namespace IMG_PROC
}

using namespace std;

int main() {
	// Read input image
	cv::Mat inputImage = cv::imread("input.bmp");
	cv::Mat forwardOutputImage, backwardOutputImage;

	// Get Homogeneous Transform Matrix
	cv::Mat mat_Scale;
	IPCVL::UTIL::createScaleTransMatrix(mat_Scale, 1.5, 1.2);

	if (inputImage.channels() > 1) {
		// Split image channels (BGR or Grayscale)
		std::vector<cv::Mat> inputChannels(3), forwardOutputChannels(3), backwardOutputChannels(3);
		cv::split(inputImage, inputChannels);

		// Manipulate image with floodfill algorithm
		for (int i = 0; i < inputImage.channels(); i++) {
			cout << "\nChannel " << i << endl;
			cout << "Forward Geometric Transformation with Scale(x1.5, x1.2x)..." << endl;
			IPCVL::IMG_PROC::forwardGeometricTransformation(inputChannels[i], forwardOutputChannels[i], mat_Scale);
			cout << "Backward Geometric Transformation with Scale(x1.5, x1.2x)..." << endl;
			IPCVL::IMG_PROC::backwardGeometricTransformation(inputChannels[i], backwardOutputChannels[i], mat_Scale);
		}

		// Merge 3 channels into outputImage
		cv::merge(forwardOutputChannels, forwardOutputImage);
		cv::merge(backwardOutputChannels, backwardOutputImage);
	}
	else {
		cout << "Forward Geometric Transformation with Scale(x1.5, x1.2x)..." << endl;
		IPCVL::IMG_PROC::forwardGeometricTransformation(inputImage, forwardOutputImage, mat_Scale);
		cout << "Backward Geometric Transformation with Scale(x1.5, x1.2x)..." << endl;
		IPCVL::IMG_PROC::backwardGeometricTransformation(inputImage, backwardOutputImage, mat_Scale);
	}

	cv::imwrite("user_out_forwardgt.bmp", forwardOutputImage);
	cv::imwrite("user_out_backwardgt.bmp", backwardOutputImage);

	// 아래 코드는 채점시에는 작동되지 않습니다.
#ifndef _JUDGE
	//Show Result

	cv::namedWindow("Source Image", cv::WINDOW_NORMAL);
	cv::namedWindow("Forward-GT Output Image", cv::WINDOW_NORMAL);
	cv::namedWindow("Backward-GT reverse-pixelmatch Output Image", cv::WINDOW_NORMAL);

	cv::imshow("Source Image", inputImage);
	cv::imshow("Forward-GT Output Image", forwardOutputImage);
	cv::imshow("Backward-GT reverse-pixelmatch Output Image", backwardOutputImage);
	cv::waitKey();
#endif

	return 0;
}

