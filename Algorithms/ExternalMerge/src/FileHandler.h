#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Data.h"

using namespace std;

namespace EXSORT {
	namespace FILEHANDLER {
		void copyStringToArray(char *data, const string& token);
		void processing_stringLine_to_Array(Data *data_array, const string& line, int index);
		void fileLines_to_dataArray_as_counts(ifstream& input_file_descriptor, Data *data, int number_of_lines);
		void write_data_to_file(ofstream& out_file_descriptor, Data *data, int numberOfData);
		int readLine_fromFile(const string& filepath);
		void transfer_fileData_to_dataArray(ifstream buffer_file_descriptor[], Data buffer[], int numberOfFile, int &number_of_prevFile,
			int step, int number_of_files, const string& basepath, int& num_of_current_files);
		void make_output_file(ofstream &outFd, ifstream bufferInFd[], Data buffer[], int memorySize);
	}
}