//
//  main.cpp
//  ipcvlab_assignment
//
//  Created by Hangeul Bae on 05/03/2019.
//  Copyright © 2019 Hangeul Bae. All rights reserved.
//

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

using namespace std;
using namespace cv;

namespace IPCVL {
    namespace UTIL {
        // 가우시안 함수
        double gaussian(int y, int x, double stdDev) {
            return (1.0 / (2 * M_PI * pow(stdDev, 2)))
            * pow(M_E, -((pow(y, 2) + pow(x, 2)) / (2 * pow(stdDev, 2))));
        }
        
        // 라디안(Radian)을 디그리(Degree)로 변환한다.
        // atan()함수에서 반환하는 값은 라디안(Radian)이다.
        double radianToDegree(double radian) {
            return radian * 180.0 / M_PI;
        }
        
        // 디그리(Degree)를 라디안(Radian)으로 변환한다.
        // -90도는 라디안으로 약 -1.5708이다.
        double degreeToRadian(double degree) {
            return degree / 180.0 * M_PI;
        }
        
        // 기본적으로 8, 0부터 quantizeCount-1 까지의 숫자를 반환한다.
        int quantizeRadian(double radian, int quantizeCount) {
            double degree = radianToDegree(radian);
            int count = floor((degree < 0.0 ? (degree + 360.0) : degree) / 360.0 * quantizeCount);
            return count;
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
        
        // 이미지를 반전한다.
        // CV_8UC1만 이용한다.
        void inverseImage(cv::Mat image) {
            for (int j = 0; j < image.rows; j++)
                for (int i = 0; i < image.cols; i++)
                    image.at<uchar>(j, i) = 255 - image.at<uchar>(j, i);
        }
    } // namespace UTIL
    
    namespace IMG_PROC {
        // 가우시안 스무싱 필터를 만드는 함수
        // 입력으로 주어진 가우시안 표준편차를 이용합니다.
        // 필터는 항상 홀수 크기로 만들어진다고 가정합니다 (교재 내용 참조)
        // Hint: 이전 Assginemnt #4의 함수를 그대로 사용합니다.
        void calculateGaussianSmoothingFilter(double gaussianStdDev, cv::OutputArray gaussianFilter) {
            int filterSize = ceil(gaussianStdDev * 6); // integer number bigger than six sigma
            filterSize = (filterSize % 2 == 1) ? filterSize : filterSize + 1; // and odd
            
            gaussianFilter.create(filterSize, filterSize, CV_64FC1);
            cv::Mat gaussianMat = gaussianFilter.getMat();
            
            for (int j = 0; j < gaussianMat.rows; j++)
                for (int i = 0; i < gaussianMat.cols; i++)
                    gaussianMat.at<double>(j, i) = UTIL::gaussian(j - floor(gaussianMat.rows / 2), i - floor(gaussianMat.cols / 2), gaussianStdDev);
        }
        
        // 입력 이미지의 Edge 강도와 Direction을 가져오는 함수
        // 여기서는 Sobel Operator (pp. 118,119 참조)를 이용해서 Strength, Direction Map을 생성합니다.
        // Hint 1: pp. 118을 참조하여 소벨 마스크를 제작합니다.
        // Hint 2: pp. 119를 참조하여 에지 강도와 "그레이디언트 방향'을 구합니다.
        // Hint 3: 구한 그레이디언트 방향에 -90deg를 해줌으로써 에지 방향을 구합니다.
        // Hint 4: degree => radian이나 radian => degree는 UTIL함수의 degreeToRadian이나 radianToDegree를 이용합니다.
        // Hint 5: atan() 함수에서 밑으로 들어가는 d_x의 경우 0일 수 있습니다. 0이면 y축 값만 존재하므로 방향도 2입니다.
        void doExtractEdgeAttributes(cv::InputArray inputImage, cv::OutputArray edgeStrength, cv::OutputArray gradientDirection) {
            double sobelMy[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};
            double sobelMx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
            cv::Mat sobelMyMat = cv::Mat(3,3,CV_64FC1,sobelMy);
            cv::Mat sobelMxMat = cv::Mat(3,3,CV_64FC1,sobelMx);
            
            cv::Mat srcMat = inputImage.getMat();
            cv::Mat edgeStrengthMat = cv::Mat::zeros(srcMat.size(), srcMat.type());
            cv::Mat gradientDirectionMat = cv::Mat::zeros(srcMat.size(), srcMat.type());
            
            edgeStrength.assign(edgeStrengthMat);
            gradientDirection.assign(gradientDirectionMat);
            
            cv::Mat dyMat = cv::Mat::zeros(srcMat.size(), srcMat.type());
            cv::Mat dxMat = cv::Mat::zeros(srcMat.size(), srcMat.type());
            
            cv::filter2D(srcMat, dyMat, -1, sobelMyMat);
            cv::filter2D(srcMat, dxMat, -1, sobelMxMat);
            
            for(int y=0;y<dyMat.rows;y++){
                for(int x=0;x<dyMat.cols;x++){
                    edgeStrengthMat.at<double>(y,x) = sqrt(pow(dyMat.at<double>(y,x), 2)+pow(dxMat.at<double>(y,x), 2));
                    if(dxMat.at<double>(y,x) == 0) {
                        gradientDirectionMat.at<double>(y,x) = 2;
                    } else {
                        double radian = atan(dyMat.at<double>(y,x)/dxMat.at<double>(y,x)) + 1.5708 ;
                        gradientDirectionMat.at<double>(y,x) = (double)IPCVL::UTIL::quantizeRadian(radian,8);
                    }
                }
            }
        }
        
        // 비최대 억제 함수
        // strengthMap은 InputArray지만 가져온 Mat객체는 write할 수 있습니다. 이부분은 신경쓰지 않아도 됩니다.
        // (정정) directionMap을 이용해서 가져온 에지 방향으로 비교를 합니다.
        void nonMaximumSuppression(cv::InputArray strengthMap, cv::InputArray directionMap) {
            cv::Mat strengthMat = strengthMap.getMat();
            cv::Mat directionMat = directionMap.getMat();
            
            for(int y=0; y<directionMat.rows;y++){
                for(int x=0; x<directionMat.cols;x++){
                    int direction = directionMat.at<double>(y,x);
                    if(direction % 4 == 0) {
                        if(strengthMat.at<double>(y,x) <= strengthMat.at<double>(y-1,x) ||
                           strengthMat.at<double>(y,x) <= strengthMat.at<double>(y+1,x)){
                            strengthMat.at<double>(y,x) = 0;
                        }
                    }
                    if(direction % 4 == 1) {
                        if(strengthMat.at<double>(y,x) <= strengthMat.at<double>(y-1,x+1) ||
                           strengthMat.at<double>(y,x) <= strengthMat.at<double>(y+1,x-1)){
                            strengthMat.at<double>(y,x) = 0;
                        }
                    }
                    if(direction % 4 == 2) {
                        if(strengthMat.at<double>(y,x) <= strengthMat.at<double>(y,x-1) ||
                           strengthMat.at<double>(y,x) <= strengthMat.at<double>(y,x+1)){
                            strengthMat.at<double>(y,x) = 0;
                        }
                    }
                    if(direction % 4 == 3) {
                        if(strengthMat.at<double>(y,x) <= strengthMat.at<double>(y-1,x-1) ||
                           strengthMat.at<double>(y,x) <= strengthMat.at<double>(y+1,x+1)){
                            strengthMat.at<double>(y,x) = 0;
                        }
                    }
                }
            }
        }
        
        // 이력 임계값 함수 (pp. 133~134 참고)
        // 주어진 에지 맵(edgeMap)으로부터 추적해서, Tlow < edge < Thigh인 에지들만 결과 이미지(outputImage)로 "복사"합니다.)
        // edge는 이전에 만든 얇은 두께 에지 맵이며, 이것과 방문 여부(visited)를 이용해서 출력 이미지(outputImage)를 채우는 것입니다.
        // 교재에서는 Recursive function을 사용했지만 Stack Overflow 문제로 인해 "Queue로 대체해 해결"합니다.
        // Hint 1: 주어지는 Thigh, Tlow (Threshold parameter)들은 0부터 1까지의 값을 가집니다. 이미지의 강도(0~255)로 변환합니다.
        // Hint 2: 결과 이미지와 새로 생성하는 "visited"(방문여부 확인)을 Mat으로 제작할 경우, 모든 값을 반드시 0으로
        //         초기화를 해준 상태에서 시작해야 합니다. 기본적으로 만들었을 때의 값은 "-6.27744e+66"와 같은 작은 값이며 0이 아닙니다.
        void doHysteresisThresholding(cv::InputArray edgeMap, cv::OutputArray outputImage, double Tlow, double Thigh) {
            Tlow *= 255;
            Thigh *= 255;
            queue<int> follow_edege;
            
            cv::Mat edgeMat = edgeMap.getMat();
            cv::Mat dstMat = cv::Mat::zeros(edgeMat.size(), edgeMat.type());
            outputImage.assign(dstMat);
            
            cv::Mat visited = cv::Mat::zeros(edgeMat.size(), edgeMat.type());

            for(int Y=0; Y<edgeMat.rows-1;Y++){
                for(int X=0; X<edgeMat.cols-1;X++){
                    if(visited.at<double>(Y,X) == 0 && edgeMat.at<double>(Y,X)>Thigh){
                        follow_edege.push(Y);
                        follow_edege.push(X);
                        while(!follow_edege.empty()){
                            int y = follow_edege.front();
                            follow_edege.pop();
                            int x = follow_edege.front();
                            follow_edege.pop();
                            //visited.at<double>(y,x) = 1;
                            //dstMat.at<double>(y,x) = 255;
                            if(visited.at<double>(y,x)==0){
                                visited.at<double>(y,x) = 1;
                                dstMat.at<double>(y,x) = 255;
                                for(int j=-1; j<2; j++) {
                                    for(int i=-1; i<2; i++) {
                                        if(edgeMat.at<double>(y+j,x+i)>Tlow) {
                                            follow_edege.push(y+j);
                                            follow_edege.push(x+i);
                                        }
                                    }
                                }
                            }
                        }
                    }//end of while
                }//end of for loop1
            }//end of for loop2
        }//end of func
        
        // 캐니 에지 검출 함수
        // 캐니 에지를 검출해서, 결과 이미지(outputImage)로 나타냅니다.
        void doCannyEdgeDetection(cv::InputArray inputImage, cv::OutputArray outputFalsePositiveImage, cv::OutputArray outputImage, double gaussianStdDev, double Tlow, double Thigh) {
            cv::Mat inputMat = inputImage.getMat();
            outputImage.create(inputMat.size(), inputMat.type());
            outputFalsePositiveImage.create(inputMat.size(), inputMat.type());
            cv::Mat outputMat = outputImage.getMat();
            cv::Mat outputFalsePositiveMat = outputFalsePositiveImage.getMat();
            
            // 1. 가우시안 스무딩을 적용한다.
            cv::Mat gaussianMat, gaussianOutput;
            calculateGaussianSmoothingFilter(gaussianStdDev, gaussianMat);
            cv::filter2D(inputImage, gaussianOutput, -1, gaussianMat);
            
            // 2. 결과 영상에 소벨 연산자를 이용하여 에지 강도와 에지 방향 맵을 구한다.
            cv::Mat edgeStrengthMap, edgeDirectionMap;
            doExtractEdgeAttributes(gaussianOutput, edgeStrengthMap, edgeDirectionMap);
            
            // 3. 비최대 억제를 적용하여 얇은 두께 에지 맵을 만든다.
            nonMaximumSuppression(edgeStrengthMap, edgeDirectionMap);
            edgeStrengthMap.copyTo(outputFalsePositiveMat); // 중간에 내보내지 않으므로 현재 값들을 복사한다. (정답인지 확인을 위해...)
            
            // 4. 이력 임계값을 적용하여 거짓 긍정을 제거한다.
            doHysteresisThresholding(edgeStrengthMap, outputImage, Tlow, Thigh);
        }
    }  // namespace IMG_PROC
}

int main() {
    // Read input image
    // 이번 과제는 CV_64FC1으로 Type을 통일합니다.
    cv::Mat inputImage = cv::imread("input.bmp", cv::IMREAD_GRAYSCALE); // 반드시 채점시에는 input.bmp로 변경해주세요.
    inputImage.convertTo(inputImage, CV_64FC1); // 변환된 형 재확인.
    cv::Mat fpOutputImage, outputImage; // fpOutputImage => false positive가 있는 이미지. 채점 확인용.
    
    // 캐니 에지 검출을 수행합니다.
    IPCVL::IMG_PROC::doCannyEdgeDetection(inputImage, fpOutputImage, outputImage, 1.5, 0.2, 0.5);
    
    // CV_64FC1이 파일/화면 출력시 항상 문제를 발생함...
    inputImage.convertTo(inputImage, CV_8UC1);
    fpOutputImage.convertTo(fpOutputImage, CV_8UC1);
    outputImage.convertTo(outputImage, CV_8UC1);
    
    // 보기쉽게 반전한다.
    IPCVL::UTIL::inverseImage(fpOutputImage);
    IPCVL::UTIL::inverseImage(outputImage);
    
    cv::imwrite("user_out_fp.bmp", fpOutputImage); // 반드시 채점 돌리기 전 파일이 잘 출력되었는지 확인해주세요.
    cv::imwrite("user_out.bmp", outputImage); // 반드시 채점 돌리기 전 파일이 잘 출력되었는지 확인해주세요.
    
    // 아래 코드는 채점시에는 작동되지 않습니다.
#ifndef _JUDGE
    // 결과 확인 코드
    // 마지막 False Positive가 제거되는 과정을 확인할 수 있다.
    cv::namedWindow("Source Image", cv::WINDOW_FREERATIO);
    cv::namedWindow("Canny Edge Detection Image => has FP", cv::WINDOW_FREERATIO);
    cv::namedWindow("Canny Edge Detection Image => without FP", cv::WINDOW_FREERATIO);
    
    cv::imshow("Source Image", inputImage);
    cv::imshow("Canny Edge Detection Image => has FP", fpOutputImage);
    cv::imshow("Canny Edge Detection Image => without FP", outputImage);
    
    cv::resizeWindow("Source Image", cv::Size(inputImage.cols * 2, inputImage.rows * 2)); // 이미지의 크기 2배에 딱 맞게 조절한다
    cv::resizeWindow("Canny Edge Detection Image => has FP", cv::Size(fpOutputImage.cols * 2, fpOutputImage.rows * 2));
    cv::resizeWindow("Canny Edge Detection Image => without FP", cv::Size(outputImage.cols * 2, outputImage.rows * 2));
    
    cv::waitKey(); // waitKey는 마지막에 한번만 사용
    // 결과 확인 코드 끝
#endif
    
    return 0;
}
