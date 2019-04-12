#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;

int main() {
    string inputFilename, outputFilename;
    cin >> inputFilename >> outputFilename;

    // Read input image
    cv::Mat inputImage = cv::imread(inputFilename, cv::IMREAD_GRAYSCALE);

    for (int y = 0; y < inputImage.rows; y++)
        for (int x = 0; x < inputImage.cols; x++)
            inputImage.at<uchar>(y, x) =
                cv::saturate_cast<uchar>(1.1 * inputImage.at<uchar>(y, x) + 50);

    // Write output image
    cv::imwrite(outputFilename, inputImage);
    return 0;
}