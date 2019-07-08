#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;

namespace IPCVL {
    namespace Process {
        void calcHist(cv::InputArray src, int* histogram) {
            cv::Mat inputMat = src.getMat();

            for (int y = 0; y < inputMat.rows; y++) {
                for (int x = 0; x < inputMat.cols; x++) {
                    // Todo : histogram을 쌓습니다.

                    /** your code here! **/
                    histogram[inputMat.at<uchar>(y, x)]++;
                    // hint 1 : for loop 를 이용해서 cv::Mat 순회 시 (1채널의 경우) 
                    // inputMat.at<uchar>(y, x)와 같이 데이터에 접근할 수 있습니다. 
                }
            }
        }
    }
    namespace Util {
        void ChangeContrastAndBrightness(cv::InputArray src, cv::OutputArray dst, double alpha, int beta) {
            dst.create(src.size(), src.type());
            cv::Mat inputMat = src.getMat();
            cv::Mat outputMat = dst.getMat();

            for (int y = 0; y < inputMat.rows; y++)
                for (int x = 0; x < inputMat.cols; x++)
                    outputMat.at<uchar>(y, x) = cv::saturate_cast<uchar>(alpha * inputMat.at<uchar>(y, x) + beta);
        }

        void GetHistogramImage(int* histogram, cv::OutputArray dst, int hist_w, int hist_h) {
            dst.create(cv::Size(hist_w, hist_h), CV_8UC1);
            cv::Mat histImage = dst.getMat();
            histImage.setTo(cv::Scalar(255, 255, 255));

            int bin_w = cvRound((double)hist_w / 256);

            // find the maximum intensity element from histogram
            int max = histogram[0];

            for (int i = 1; i < 256; i++)
                if (max < histogram[i]) max = histogram[i];

            // normalize the histogram between 0 and histImage.rows
            for (int i = 0; i < 255; i++)
                histogram[i] = ((double)histogram[i] / max) * histImage.rows;

            // draw the intensity line for histogram
            for (int i = 0; i < 255; i++)
                cv::line(histImage, cv::Point(bin_w*(i), hist_h), cv::Point(bin_w*(i), hist_h - histogram[i]), cv::Scalar(0, 0, 0), 1, 8, 0);
        }
    }
}

int main() {
    string inputFilename, outputFilename;
    cin >> inputFilename >> outputFilename;

    cv::Mat inputImage = cv::imread(inputFilename, cv::IMREAD_GRAYSCALE), outputImage;
    if (inputImage.empty()) {
        cerr << "Input Empty" << endl;
        return 1;
    }

    double alpha = 1.1;
    int beta = 50;
    IPCVL::Util::ChangeContrastAndBrightness(inputImage, outputImage, alpha, beta);
    if (outputImage.empty()) {
        cerr << "Empty output image" << endl;
        return 1;
    }

    int inputHistogram[256] = { 0, };
    int outputHistogram[256] = { 0, };

    // Todo : namespace IPCVL::Process에 있는 calcHist 함수를 작성하세요
    IPCVL::Process::calcHist(inputImage, inputHistogram);
    IPCVL::Process::calcHist(outputImage, outputHistogram);

    cv::Mat inputHistImage;
    cv::Mat outputHistImage;

    int hist_w = 512, hist_h = 400;
    IPCVL::Util::GetHistogramImage(inputHistogram, inputHistImage, hist_w, hist_h);
    IPCVL::Util::GetHistogramImage(outputHistogram, outputHistImage, hist_w, hist_h);

    // 아래 코드는 채점시에는 작동되지 않습니다.

#ifndef _JUDGE
    cv::imshow("Input file", inputImage);
    cv::imshow("Output file", outputImage);
    cv::imshow("Input file Histogram", inputHistImage);
    cv::imshow("Output file Histogram", outputHistImage);
    cv::waitKey(0);
#endif

    cv::imwrite(outputFilename, outputHistImage);
    return 0;
}