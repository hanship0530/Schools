#include "Geometry.h"

void CGeometry::MatrixMul(float homoMatrix[3][3])
{
	if(!initMat)
	{
		for(int i = 0; i < 3; ++i)
	        for(int j = 0; j < 3; ++j)
				HMatrix[i][j] = homoMatrix[i][j];  
		initMat = true;
		return;
	}
	
	float mult[3][3] = {0., };

	for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
            for(int k = 0; k < 3; ++k)            
                mult[i][j] += HMatrix[i][k] * homoMatrix[k][j];

	for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
			HMatrix[i][j] = mult[i][j];           
}


void CGeometry::MatrixMul(float homoCoord[1][3], float outputCoord[1][3])
{	
	for(int i = 0; i < 1; ++i)
        for(int j = 0; j < 3; ++j)
            for(int k = 0; k < 3; ++k)            
                outputCoord[i][j] += homoCoord[i][k] * HMatrix[k][j];
}

void CGeometry::ForwardingMapping(cv::Mat &src, cv::Mat &dst)
{
	float homoCoord[1][3] = {0., };	
	for(int y = 0; y < src.rows; ++y)
	{
		for(int x = 0; x < src.cols; ++x)
		{
            float outputCoord[1][3] = {0., };
            //Make ForwardingMapping
            homoCoord[0][0] = y;
            homoCoord[0][1] = x;
            homoCoord[0][2] = 1;
            MatrixMul(homoCoord, outputCoord);
            int yp = round(outputCoord[0][0]);
            int xp = round(outputCoord[0][1]);
            if(yp>=0&&yp<dst.rows&&xp>=0&&xp<dst.cols){
                dst.at<uchar>(yp,xp) = src.at<uchar>(y,x);
            }
		}
	}
}

void CGeometry::BackwardingMapping(cv::Mat &src, cv::Mat &dst)
{
	float homoCoord[1][3] = {0., };	
	for(int y = 0; y < src.rows; ++y)
	{
		for(int x = 0; x < src.cols; ++x)
		{
			//Make BackwardingMapping
            float outputCoord[1][3] = {0., };
            homoCoord[0][0] = y;
            homoCoord[0][1] = x;
            homoCoord[0][2] = 1;
            MatrixMul(homoCoord, outputCoord);
            int yp = round(outputCoord[0][0]);
            int xp = round(outputCoord[0][1]);
            if(yp>=0&&yp<dst.rows&&xp>=0&&xp<dst.cols){
                dst.at<uchar>(y,x) = src.at<uchar>(yp,xp);
            }
		}
	}
}

void CGeometry::Move(const int y, const int x)
{
	float MoveHomoMatrix[3][3];

	MoveHomoMatrix[0][0] = 1;
	MoveHomoMatrix[0][1] = 0;
	MoveHomoMatrix[0][2] = 0;

	MoveHomoMatrix[1][0] = 0;
	MoveHomoMatrix[1][1] = 1;
	MoveHomoMatrix[1][2] = 0;

	MoveHomoMatrix[2][0] = y;
	MoveHomoMatrix[2][1] = x;
	MoveHomoMatrix[2][2] = 1;
	MatrixMul(MoveHomoMatrix);
}

void CGeometry::Rotate(const int degree)
{
	float RotateHomoMatrix[3][3];
	double radius = GetDadius(degree);
	// Make RotateHomoMatrix about Rotation
    RotateHomoMatrix[0][0] = cos(radius);
    RotateHomoMatrix[0][1] = -sin(radius);
    RotateHomoMatrix[0][2] = 0;
    RotateHomoMatrix[1][0] = sin(radius);
    RotateHomoMatrix[1][1] = cos(radius);
    RotateHomoMatrix[1][2] = 0;
    RotateHomoMatrix[2][0] = 0;
    RotateHomoMatrix[2][1] = 0;
    RotateHomoMatrix[2][2] = 1;
	MatrixMul(RotateHomoMatrix);
}

void CGeometry::Inverse_Rotate(const int degree)
{
	float RotateHomoMatrix[3][3];
	double radius = GetDadius(degree);
	// Make InverseRotateHomoMatrix about Rotation
    RotateHomoMatrix[0][0] = cos(radius);
    RotateHomoMatrix[0][1] = sin(radius);
    RotateHomoMatrix[0][2] = 0;
    RotateHomoMatrix[1][0] = -sin(radius);
    RotateHomoMatrix[1][1] = cos(radius);
    RotateHomoMatrix[1][2] = 0;
    RotateHomoMatrix[2][0] = 0;
    RotateHomoMatrix[2][1] = 0;
    RotateHomoMatrix[2][2] = 1;
	MatrixMul(RotateHomoMatrix);
}
