#include <iostream>
#include <queue>
#include <tuple>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <cmath>

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
			// Hint 1: Mat객체간의 행렬곱을 하기 위해서는 * 연산자를 사용한다.
			// Hint 2: 배열을 Mat객체로 변환하기 위해서는 다음과 같이 쓴다:
			/*
				cv::Mat arrayToMat(<rows>, <cols>, CV_64FC1, <double type array>);
			*/

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
						|| (int)axis.at<double>(0, 1) < 0 || (int)axis.at<double>(0, 1) >= dstMat.cols || axis.at<double>(0, 2) != 1) {
						continue;
					}
					dstMat.at<char>((int)axis.at<double>(0, 0), (int)axis.at<double>(0, 1)) = srcMat.at<char>(y, x);
				}
			}
		}
	}  // namespace IMG_PROC
}

using namespace std;

int main() {
	// Read input image
	cv::Mat inputImage = cv::imread("input.bmp");
	cv::Mat rotateOutputImage, scaleOutputImage;

	// Get Homogeneous Transform Matrix
	cv::Mat mat_Rotate, mat_Scale;
	IPCVL::UTIL::createRotateTransMatrix(mat_Rotate, 15);
	IPCVL::UTIL::createScaleTransMatrix(mat_Scale, 1.2, 1.5);

	if (inputImage.channels() > 1) {
		// Split image channels (BGR or Grayscale)
		std::vector<cv::Mat> inputChannels(3), rotateOutputChannels(3), scaleOutputChannels(3);
		cv::split(inputImage, inputChannels);

		// Manipulate image with floodfill algorithm
		for (int i = 0; i < inputImage.channels(); i++) {
			cout << "\nChannel " << i << endl;
			cout << "Forward Geometric Transformation with Rotate(15deg)..." << endl;
			IPCVL::IMG_PROC::forwardGeometricTransformation(inputChannels[i], rotateOutputChannels[i], mat_Rotate);
			cout << "Forward Geometric Transformation with Scale(x1.2, x1.5)..." << endl;
			IPCVL::IMG_PROC::forwardGeometricTransformation(inputChannels[i], scaleOutputChannels[i], mat_Scale);
		}

		// Merge 3 channels into outputImage
		cv::merge(rotateOutputChannels, rotateOutputImage);
		cv::merge(scaleOutputChannels, scaleOutputImage);
	}
	else {
		cout << "Forward Geometric Transformation with Rotate(15deg)..." << endl;
		IPCVL::IMG_PROC::forwardGeometricTransformation(inputImage, rotateOutputImage, mat_Rotate);
		cout << "Forward Geometric Transformation with Scale(x2, x1.5)..." << endl;
		IPCVL::IMG_PROC::forwardGeometricTransformation(inputImage, scaleOutputImage, mat_Scale);
	}

	cv::imwrite("user_out_forwardgt_rotate.bmp", rotateOutputImage);
	cv::imwrite("user_out_forwardgt_scale.bmp", scaleOutputImage);

	// 아래 코드는 채점시에는 작동되지 않습니다.
#ifndef _JUDGE
	//Show Result

	cv::namedWindow("Source Image", cv::WINDOW_NORMAL);
	cv::namedWindow("Forward-GT Rotate Output Image", cv::WINDOW_NORMAL);
	cv::namedWindow("Forward-GT Scale Output Image", cv::WINDOW_NORMAL);

	cv::resizeWindow("Source Image", 600, 800);
	cv::resizeWindow("Forward-GT Rotate Output Image", 600, 800);
	cv::resizeWindow("Forward-GT Scale Output Image", 600, 800);

	cv::imshow("Source Image", inputImage);
	cv::imshow("Forward-GT Rotate Output Image", rotateOutputImage);
	cv::imshow("Forward-GT Scale Output Image", scaleOutputImage);
	cv::waitKey();
#endif

	return 0;
}