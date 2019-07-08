#include "Describe.h"
#include <iostream>
Describe::Describe() {}

bool Describe::sortbysec(const pair<int, float> &a,
	const pair<int, float> &b)
{
	return (a.second < b.second);
}

vector<pair<Point, vector<float>>> Describe::get_describe_keypoints() {
	return describe_keypoints;
}

vector<pair<int, int>> Describe::get_matches() {
	return matches;
}

void Describe::make_describe_keypoints(const cv::Mat &src, const std::vector<cv::Point> &key_points, const int patch_size) {
	const int n_bins = 9;
	int degrees_per_bin = 180 / n_bins;
	int maskX[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} };
	int maskY[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} };
	float **d_cells = new float *[patch_size];
	float **theta_cells = new float *[patch_size];
	float ***cells = new float **[patch_size / (patch_size / 2)];
	for (int col = 0; col < patch_size; col++) {
		d_cells[col] = new float[patch_size];
		theta_cells[col] = new float[patch_size];
		memset(d_cells[col], 0.0, sizeof(d_cells[col]));
		memset(theta_cells[col], 0.0, sizeof(theta_cells[col]));
	}
	for (int row = 0; row < patch_size / (patch_size / 2); row++) {
		cells[row] = new float *[patch_size / (patch_size / 2)];
		for (int in_row = 0; in_row < patch_size / (patch_size / 2); in_row++) {
			cells[row][in_row] = new float[n_bins];
			memset(cells[row][in_row], .0, sizeof(cells[row][in_row]));
		}
	}
	cout << "key_points size: " << key_points.size() << endl;
	for (int i = 0; i < key_points.size(); i++) {
		int y = key_points[i].y;
		int x = key_points[i].x;
		vector<float> feactureVect;
		for (int row = 0; row < patch_size; row++) {
			memset(d_cells[row], 0.0, sizeof(d_cells[row]));
			memset(theta_cells[row], 0.0, sizeof(theta_cells[row]));
		}
		for (int row = 0; row < patch_size / (patch_size / 2); row++) {
			for (int in_row = 0; in_row < patch_size / (patch_size / 2); in_row++) {
				memset(cells[row][in_row], 0.0, sizeof(cells[row][in_row]));
			}
		}
		for (int h = y - patch_size / 2; h < y + (patch_size + 1) / 2; h++) {
			if (h <= 0 && h >= src.rows-1)
				continue;
			for (int w = x - patch_size / 2; w < x + (patch_size + 1) / 2; w++) {
				if (w <= 0 && w >= src.cols-1)
					continue;
				float dx = 0;
				float dy = 0;
				for (int b = -1; b < 2; b++) {

					for (int a = -1; a < 2; a++) {
						if (h + b >= 0 && h + b < src.rows && w + a >= 0 && w + a < src.cols) {
							dx += src.at<uchar>(h + b, w + a)*maskX[b + 1][a + 1];
							dy += src.at<uchar>(h + b, w + a)*maskY[b + 1][a + 1];
						}
					}
				}
				float d = sqrt(pow(dx, 2) + pow(dy, 2));
				float theta = atan2(dy, dx) * 180 / PI;
				theta = cvRound(abs(theta)) % 180;
				d_cells[h - (y - patch_size / 2)][w - (x - patch_size / 2)] = d;
				theta_cells[h - (y - patch_size / 2)][w - (x - patch_size / 2)] = theta;
			}
		}
		float sum = 0.0;
		float squareSum = 0.0;
		for (int h = 0; h < (patch_size) / (patch_size / 2); h++) {
			for (int w = 0; w < (patch_size) / (patch_size / 2); w++) {
				for (int k = h; k < h + (patch_size / 2); k++) {
					for (int l = w; l < w + (patch_size / 2); l++) {
						int idx = theta_cells[k][l] / degrees_per_bin;
						if (idx == 9)
							idx = 8;
						cells[h][w][idx] += d_cells[k + h][l + w];
						sum += d_cells[k + h][l + w];
						squareSum += pow(d_cells[k + h][l + w], 2);
					}
				}
			}
		}
		float mean = sum / 36;
		float squareMean = squareSum / 36;
		float std = sqrt(squareMean - pow(mean, 2));
		for (int h = 0; h < (patch_size) / (patch_size / 2); h++) {
			for (int w = 0; w < (patch_size) / (patch_size / 2); w++) {
				for (int k = 0; k < n_bins; k++) {
					//cells[h][w][k] = (cells[h][w][k] - mean)/std;
					if (sum != 0)
						cells[h][w][k] = cells[h][w][k] / sum;
					feactureVect.push_back(cells[h][w][k]);
				}
			}
		}
		describe_keypoints.push_back(make_pair(Point(x, y), feactureVect));
	}

	for (int col = 0; col < patch_size; col++) {
		delete[] d_cells[col];
		delete[] theta_cells[col];
	}
	for (int row = 0; row < patch_size / (patch_size / 2); row++) {
		for (int in_row = 0; in_row < patch_size / (patch_size / 2); in_row++) {
			delete[] cells[row][in_row];
		}
		delete[] cells[row];
	}
	delete[] d_cells;
	delete[] theta_cells;
	delete[] cells;
}

void Describe::match_descriptors(vector<pair<Point, vector<float>>> desc1, vector<pair<Point, vector<float>>> desc2, float threshold) {
	for (int i = 0; i < desc1.size(); i++) {
		vector<pair<float, int>> distVec;
		for (int j = 0; j < desc2.size(); j++) {
			float distance = 0;
			for (int k = 0; k < desc1[i].second.size(); k++) {
				distance += sqrt(pow(desc1[i].second[k] - desc2[j].second[k], 2));
			}
			distVec.push_back(make_pair(distance, j));
		}
		sort(distVec.begin(), distVec.end());
		if (distVec[0].first / distVec[1].first <= threshold)
			matches.push_back(make_pair(i, distVec[0].second));
	}
	for (int i = 0; i < matches.size(); i++) {
		cout << "(" << desc1[matches[i].first].first.y << ", " << desc1[matches[i].first].first.x << ") <-> ";
		cout << "(" << desc2[matches[i].second].first.y << ", " << desc2[matches[i].second].first.x << ")" << endl;
	}
	cout << "match count: " << matches.size() << endl;
}
void Describe::DrawFeature(const cv::Mat &dst1, const cv::Mat &dst2, const vector<Point> &key_point1, const vector<Point> &key_point2) {
	for (int i = 0; i < matches.size(); i++) {
		circle(dst1, key_point1[matches[i].first], 3, cv::Scalar(255, 0, 0));
		circle(dst2, key_point2[matches[i].second], 3, cv::Scalar(0, 255, 0));
	}
}