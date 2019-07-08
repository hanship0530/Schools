#pragma once
#include "SIFT.h"

class SIFT_Desc
{
public:
	SIFT_Desc(float discriptor[8])
	{
		for(int i = 0; i <8; ++i)
			this->discriptor[i] = discriptor[i];
	}
	float discriptor[8];
};

class SIFT_Descriptor
{
public:
	std::vector<SIFT_Desc> SIFT_DescVec;
	void FindDominantOrientation(SIFT &sift);
	void MakeDescriptor(SIFT &sift);
};