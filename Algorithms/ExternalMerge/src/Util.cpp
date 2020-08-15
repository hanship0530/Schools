#include "Data.h"
#include "pch.h"
#include "Util.h"
#include <string>
#include <iostream>

using namespace std;

namespace EXSORT {
	namespace UTILITY {
		int computeStep(int numberOfWholeLines, int numberOfChunkLines) {
			int steps=1;
			while ((numberOfWholeLines / numberOfChunkLines) > numberOfChunkLines) {
				steps++;
				numberOfWholeLines = (numberOfWholeLines / numberOfChunkLines);
			}
			return steps;
		}
		int compare(Data x, Data y) {
			if (x.data==NULL || y.data == NULL) {
				return -1;
			}
			return strcmp(x.data, y.data);
		}
		int partition(Data* arr, int low, int high)
		{
			int pivotIndex = (low + high) / 2;

			pivotIndex = high;

			for (int index = low; index <= high; index++)
			{
				//if (a[idx] > a[pivotIdx] && pivotIdx > idx)
				if (compare(arr[index], arr[pivotIndex]) > 0 && pivotIndex > index)
				{
					swap(arr[index], arr[pivotIndex]);
				}
				//else if (arr[idx] < arr[pivotIdx] && pivotIdx < idx)
				else if (compare(arr[index], arr[pivotIndex]) < 0 && pivotIndex < index)
				{
					swap(arr[index], arr[pivotIndex]);
				}
			}

			return pivotIndex;
		}

		void quicksort(Data* arr, int low, int high)
		{
			if (low < high)
			{
				int pivotIndex = partition(arr, low, high);

				quicksort(arr, low, pivotIndex - 1);
				quicksort(arr, pivotIndex + 1, high);
			}
		}
	}
}
