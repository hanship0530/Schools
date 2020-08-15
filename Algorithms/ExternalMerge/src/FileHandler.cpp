#include "FileHandler.h"
#include "pch.h"
#include "Data.h"
#include "Util.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace EXSORT {
	namespace FILEHANDLER {

		void copyStringToArray(char *data, const string& token) {
			for (int i = 0; i < token.length(); i++) {
				data[i] = token[i];
			}
			data[token.length()] = '\0';
		}

		void processing_stringLine_to_Array(Data *data_array, const string& line, int index) {
			string data = line;
			size_t pos = 0;
			string token;
			char delimiter = '\t';
			int tabCount = 0;

			while ((pos = data.find(delimiter)) != string::npos) {
				token = data.substr(0, pos);
				data.erase(0, pos + 1);
				if (tabCount == 0) {
					data_array[index].id = stoi(token);
				}
				else if (tabCount == 1) {
					copyStringToArray(data_array[index].data, token);
				}
				tabCount++;
			}
			data_array[index].price = stoi(data);
		}

		void fileLines_to_dataArray_as_counts(ifstream& input_file_descriptor, Data *data, int number_of_lines) {
			if (!input_file_descriptor.is_open()) {
				return;
			}
			string line;
			int index = 0;
			bool canReadMore = false;
			bool keepGoing = true;
			/*
				place readLines > 0 at first, 
				avoid to run getline(inputFile, line) -> there is a reading problem  
			*/
			while (number_of_lines > 0 && keepGoing) {
				if (getline(input_file_descriptor, line)) {
					processing_stringLine_to_Array(data, line, index++);
					number_of_lines--;
					canReadMore = true;
					continue;
				}
				canReadMore = false;
				keepGoing = false;
			}
			if (!canReadMore) {
				data[0].id = -1;
				input_file_descriptor.close();
			}
		}

		void write_data_to_file(ofstream& out_file_descriptor, Data *data, int numberOfData){
			if (!out_file_descriptor.is_open()) {
				return;
			}
			for (int i = 0; i < numberOfData; i++) {
				out_file_descriptor << data[i].id;
				out_file_descriptor << '\t';
				out_file_descriptor.write(data[i].data, std::strlen(data[i].data));
				out_file_descriptor << '\t';
				out_file_descriptor << data[i].price;
				out_file_descriptor << '\n';
			}
		}

		int readLine_fromFile(const string& filepath) {
			int lines = 0;
			string line;

			std::ifstream input_file_descriptor(filepath.c_str(), ios::in);

			if (!input_file_descriptor.is_open()) {
				std::cout << filepath << " is not opened!" << std::endl;
				return 0;
			}

			while (getline(input_file_descriptor, line)) {
				lines++;
			}
			input_file_descriptor.close();

			return lines;
		}

		void transfer_fileData_to_dataArray(ifstream buffer_file_descriptor[], Data buffer[], int numberOfFile, int &number_of_prevFile,
				int step, int number_of_files, const string& basepath, int& num_of_current_files) {
			if ((number_of_prevFile + 1) >= number_of_files) {
				return;
			}
			for (int num_file=0; num_file < numberOfFile; num_file++) {
				Data data[1];
				string buffer_filepath = basepath + to_string(step) + "_" + to_string(++number_of_prevFile) + ".txt";
				buffer_file_descriptor[num_file].open(buffer_filepath);
				EXSORT::FILEHANDLER::fileLines_to_dataArray_as_counts(buffer_file_descriptor[num_file], data, 1);
				buffer[num_file] = data[0];
				buffer[num_file].fileIndex = num_file + 1;
				num_of_current_files--;
			}
		}

		void make_output_file(ofstream &outFd, ifstream bufferInFd[], Data buffer[], int memorySize) {
			
			int flexibleMemorySize = memorySize;
			int closedFileCounter = 0;

			while (true) {
				Data data[1] = { buffer[0] };
				int file_number = buffer[0].fileIndex;
				EXSORT::FILEHANDLER::write_data_to_file(outFd, data, 1);
				EXSORT::FILEHANDLER::fileLines_to_dataArray_as_counts(bufferInFd[buffer[0].fileIndex - 1], data, 1);
				buffer[0] = data[0];
				buffer[0].fileIndex = file_number;
				if (buffer[0].id == -1) {
					swap(buffer[0], buffer[flexibleMemorySize - 1]);
					flexibleMemorySize--;
					closedFileCounter++;
				}
				if (closedFileCounter == memorySize) {
					break;
				}
				if (closedFileCounter > memorySize) {
					throw runtime_error("Error: Unexpected file closing");
					exit(0);
				}
				EXSORT::UTILITY::quicksort(buffer, 0, flexibleMemorySize - 1);
			}
		}
	}
}