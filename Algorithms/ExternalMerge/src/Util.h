#pragma once
#include "Data.h"

namespace EXSORT {
	namespace UTILITY {
		int computeStep(int numberOfWholeLines, int numberOfChunkLines);
		int compare(Data x, Data y);
		int partition(Data* arr, int low, int high);
		void quicksort(Data* arr, int low, int high);
	}
}