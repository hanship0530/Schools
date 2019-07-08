#include "SIFT.h"
#include "SIFT_Descriptor.h"
using namespace cv;
using namespace std;

void main()
{
	Mat image1;
	Mat image2;

	/// Load image
	image1 = imread("florence-1.bmp", CV_LOAD_IMAGE_GRAYSCALE);	
	image2 = imread("florence-2.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	Mat dst;
	Mat dst2;
	cvtColor(image1, dst, CV_GRAY2BGR);
	cvtColor(image2, dst2, CV_GRAY2BGR);

	SIFT sift;
	cv::GaussianBlur(image1, image1, cv::Size(0, 0), 1.6, 1.6);
	
	sift.InitSig();
	sift.MakeOctaveImg(image1);
	sift.DOG();
	sift.FindKeyPoint();
	
	SIFT sift2;
	cv::GaussianBlur(image2, image2, cv::Size(0, 0), 1.6, 1.6);
	
	sift2.InitSig();
	sift2.MakeOctaveImg(image2);
	sift2.DOG();
	sift2.FindKeyPoint();


	for(int i = 0; i < 3; ++i)
	{
		int octIdx = sift.octaveVec.size()-1;
		Octave &oct = sift.octaveVec[octIdx];

		cv::Mat resizeMat;
		resize(oct.gauImage[3], resizeMat, cv::Size(oct.gauImage[3].cols/2, oct.gauImage[3].rows/2));
		sift.MakeOctaveImg(resizeMat);
		sift.DOG();
		sift.FindKeyPoint();
	}
	
	for(int i = 0; i < 3; ++i)
	{
		int octIdx = sift2.octaveVec.size()-1;
		Octave &oct2 = sift2.octaveVec[octIdx];

		cv::Mat resizeMat;
		resize(oct2.gauImage[3], resizeMat, cv::Size(oct2.gauImage[3].cols/2, oct2.gauImage[3].rows/2));
		sift2.MakeOctaveImg(resizeMat);
		sift2.DOG();
		sift2.FindKeyPoint();
	}

	//sift.DrawKeyPoint(dst);
	//imshow("key Point1", dst);
	//
	//sift2.DrawKeyPoint(dst2);
	//imshow("key Point2", dst2);

	SIFT_Descriptor descriptor;
	descriptor.FindDominantOrientation(sift);
	descriptor.MakeDescriptor(sift);

	SIFT_Descriptor descriptor2;
	descriptor2.FindDominantOrientation(sift2);
	descriptor2.MakeDescriptor(sift2);


	cout << "(sift.keyPointVec.size()) is " << (sift.keyPointVec.size()) << endl;;

	// 첫번째 기술자만 프린트함
	//for(int i = 0; i < 4; ++i)
	//{
	//	for(int j = 0; j < 4; ++j)
	//	{
	//		for(int z = 0; z < 8; ++z)
	//		{
	//			std::cout << descriptor.SIFT_DescVec[i*4+j].discriptor[z] << " ";
	//		}
	//		std::cout << std::endl;
	//	}
	//}

	double sum = 0.0;
	double distance = 0.0;
	int KeyPoint1_Idx = 0;
	int KeyPoint2_Idx = 0;

	//유클리디안 거리
	//cout << "descriptor value" << endl;
	//cout << descriptor.SIFT_DescVec.size() << endl;

	//for (int i = 0; i < descriptor.SIFT_DescVec.size();i++) {
	//	for (int j = 0;j < 8;j++) {
	//		cout << descriptor.SIFT_DescVec[i].discriptor[j] << ", ";
	//	}
	//	cout << endl;
	//}
	//cout << "end" << endl;
	int count = 0;
	for(int k = 0; k < sift.keyPointVec.size(); k++){
		for(int l = 0; l < sift2.keyPointVec.size(); l++){
			//cout << "descriptor value" << endl;
			for(int i = 0; i < 4; ++i)
			{
				for(int j = 0; j < 4; ++j)
				{
					for(int z = 0; z < 8; ++z)
					{
						double dist = descriptor.SIFT_DescVec[i*4+j+k*4*4].discriptor[z] - descriptor2.SIFT_DescVec[i * 4 + j + l * 4 * 4].discriptor[z];
						sum += pow(dist,2);
						//cout << descriptor.SIFT_DescVec[i * 4 + j].discriptor[z] << ", " << descriptor2.SIFT_DescVec[i * 4 + j].discriptor[z]<<endl;
					}
				}
			}
			distance = sqrt(sum);
			sum = 0.0;

			std::cout <<"distance: " << distance <<  std::endl;
			std::cout << std::endl;

			//조건에 만족된 좌표에 원 그리기(SIFT.cpp의 DrawKeyPoint 함수 참고)
			if(distance < 1){
				count++;
				KeyPoint1_Idx = k;
				KeyPoint2_Idx = l;

				std::cout <<"KeyPoint1_Idx: " << KeyPoint1_Idx <<  std::endl;
				std::cout <<"KeyPoint2_Idx: " << KeyPoint2_Idx <<  std::endl;
				std::cout << std::endl;

				int scale = pow(2, sift.keyPointVec[k].octave);
				circle(dst, cv::Point(sift.keyPointVec[k].x, sift.keyPointVec[k].y)*scale, 3, cv::Scalar(0, 0, 255));


				int scale2 = pow(2, sift2.keyPointVec[l].octave);
				circle(dst2, cv::Point(sift2.keyPointVec[l].x, sift2.keyPointVec[l].y)*scale, 3, cv::Scalar(0, 0, 255));
			}
		}
	}
	cout << "Count is " << count << endl;
	imshow("key Point1", dst);
	imshow("key Point2", dst2);

	cvWaitKey();
}