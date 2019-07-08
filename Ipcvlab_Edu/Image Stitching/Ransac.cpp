#include "Ransac.h"
#include <iostream>
#include <cstring>
Ransac::Ransac() {}
void Ransac::do_ransac(const vector<Point> &keypoints1, const vector<Point> &keypoints2, vector<pair<int, int>> matches, float threshlod) {
	srand(time(0));
	int N = matches.size();
	int n_samples = N * 0.2, n_inliers = 0, temp_n;
	float temp_sum;
	vector<int> matched1, matched2, p1, p2, max_inliers, temp_max;
	H = Mat(3, 3, CV_32FC1);
	for (int i = 0; i < matches.size(); i++) {
		matched1.push_back(matches[i].first);
		matched2.push_back(matches[i].second);
	}

	for (int iter = 0; iter < 2000; iter++) {
		int n_samples = N * 0.2;
		int *randIdx = new int[n_samples];
		memset(randIdx, 0, sizeof(randIdx));
		bool keep = true;
		while (keep) {
			int re_xor = 0;
			for (int i = 0; i < n_samples; i++) {
				randIdx[i] = rand() % N;
				re_xor = re_xor ^ randIdx[i];
			}
			if (re_xor != 0) {
				keep = false;
			}
		}
		p1.clear();
		p2.clear();
		for (int i = 0; i < n_samples; i++) {
			p1.push_back(randIdx[i]);
			p2.push_back(randIdx[i]);
		}
		//p2를 p1에 맞게 좌표 변환
		make_affine_matrix(p2, p1, H, keypoints2, matched2, keypoints1, matched1);
		temp_n = 0;
		temp_max.clear();
		for (int i = 0; i < N; i++) {
			int tempP[3] = { 0, };
			multiMat(H, keypoints2[matched2[i]], tempP);
			temp_sum = 0.0;
			//변환된 p2의 좌표가 실제 p1과 얼마나 차이가 나는지 비교
			temp_sum = sqrt(pow(keypoints1[matched1[i]].y - tempP[0], 2) + pow(keypoints1[matched1[i]].x - tempP[1], 2));
			if (temp_sum < threshlod) {
				temp_max.push_back(i);
				//cout << temp_sum << endl;
			}
		}
		temp_n = 0;
		for (int i = 0; i < temp_max.size(); i++) {
			temp_n += temp_max[i];
		}
		if (temp_n > n_inliers) {
			max_inliers.clear();
			for (int i = 0; i < temp_max.size(); i++) {
				max_inliers.push_back((int)temp_max[i]);
			}
			n_inliers = temp_n;
		}
		delete[] randIdx;
	}
	cout << "max_inliers size: " << max_inliers.size() << endl;
	make_affine_matrix(max_inliers, max_inliers, H, keypoints2, matched2, keypoints1, matched1);
	for (int i = 0; i < max_inliers.size(); i++) {
		ransac_matches.push_back(matches[max_inliers[i]]);
		cout << ransac_matches[i].first << ", " << ransac_matches[i].second << endl;
	}
	cout << "H matrix" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << H.at<float>(i, j) << " ";
		}
		cout << endl;
	}
}

void Ransac::multiMat(Mat H, const Point p, int *returnP) {
	int tempP[3];
	tempP[0] = p.y;
	tempP[1] = p.x;
	tempP[2] = 1;
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			returnP[j] += H.at<float>(i,j) * tempP[i];
		}
	}
}
//p1의 좌표를 p2의 좌표로 변환
void Ransac::make_affine_matrix(vector<int> p1, vector<int> p2, Mat H, const vector<Point> &keypoints1, const vector<int> matched1, const vector<Point> &keypoints2, const vector<int> matched2) {
	Mat A(6, 6, CV_32FC1, Scalar(0));
	Mat B(6, 1, CV_32FC1, Scalar(0));
	float sumSqueareA1 = 0, sumSqueareA2 = 0, sumA1 = 0, sumA2 = 0, multiSumA1A2 = 0, multiSumA1B1 = 0;
	float multiSumA2B1 = 0, sumB1 = 0, multiSumA1B2 = 0, multiSumA2B2 = 0, sumB2 = 0;
	for (int i = 0; i < p1.size(); i++) {
		sumSqueareA1 += pow(keypoints1[matched1[p1[i]]].y, 2);
		sumSqueareA2 += pow(keypoints1[matched1[p1[i]]].x, 2);
		sumA1 += keypoints1[matched1[p1[i]]].y;
		sumA2 += keypoints1[matched1[p1[i]]].x;
		multiSumA1A2 += keypoints1[matched1[p1[i]]].y * keypoints1[matched1[p1[i]]].x;
		multiSumA1B1 += keypoints1[matched1[p1[i]]].y * keypoints2[matched2[p2[i]]].y;
		multiSumA2B1 += keypoints1[matched1[p1[i]]].x * keypoints2[matched2[p2[i]]].y;
		sumB1 += keypoints2[matched2[p2[i]]].y;
		multiSumA1B2 += keypoints1[matched1[p1[i]]].y * keypoints2[matched2[p2[i]]].x;
		multiSumA2B2 += keypoints1[matched1[p1[i]]].x * keypoints2[matched2[p2[i]]].x;
		sumB2 += keypoints2[matched2[p2[i]]].x;
	}
	A.at<float>(0, 0) = sumSqueareA1;
	A.at<float>(3, 3) = sumSqueareA1;
	A.at<float>(0, 1) = multiSumA1A2;
	A.at<float>(3, 4) = multiSumA1A2;
	A.at<float>(0, 2) = sumA1;
	A.at<float>(3, 5) = sumA1;
	A.at<float>(1, 0) = multiSumA1A2;
	A.at<float>(4, 3) = multiSumA1A2;
	A.at<float>(1, 1) = sumSqueareA2;
	A.at<float>(4, 4) = sumSqueareA2;
	A.at<float>(1, 2) = sumA2;
	A.at<float>(4, 5) = sumA2;
	A.at<float>(2, 0) = sumA1;
	A.at<float>(5, 3) = sumA1;
	A.at<float>(2, 1) = sumA2;
	A.at<float>(5, 4) = sumA2;
	A.at<float>(2, 2) = p1.size();
	A.at<float>(5, 5) = p1.size();

	B.at<float>(0, 0) = multiSumA1B1;
	B.at<float>(1, 0) = multiSumA2B1;
	B.at<float>(2, 0) = sumB1;
	B.at<float>(3, 0) = multiSumA1B2;
	B.at<float>(4, 0) = multiSumA2B2;
	B.at<float>(5, 0) = sumB2;

	A = A.inv();
	float tempH[6] = { 0., };
	for (int j = 0; j < 6; j++) {
		for (int i = 0; i < 6; i++) {
			tempH[j] += A.at<float>(j,i) * B.at<float>(i,0);
		}
	}
	H.at<float>(0, 0) = tempH[0];
	H.at<float>(1, 0) = tempH[1];
	H.at<float>(2, 0) = tempH[2];
	H.at<float>(0, 1) = tempH[3];
	H.at<float>(1, 1) = tempH[4];
	H.at<float>(2, 1) = tempH[5];
	H.at<float>(0, 2) = 0;
	H.at<float>(1, 2) = 0;
	H.at<float>(2, 2) = 1;
	/*
	cout << "H matrix" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << H.at<float>(i, j) << " ";
		}
		cout << endl;
	}
	*/
}

Mat Ransac::getH()
{
	return H;
}

vector<pair<int, int>> Ransac::get_ransac_matches()
{
	return ransac_matches;
}
