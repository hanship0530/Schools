// externalMergeSecond.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "Data.h"
#include "FileHandler.h"
#include "Util.h"
#include <iostream>
#include <string>

#define MEMORY_SIZE 7

using namespace std;

int main(int argc, char** argv)
{
	// variable for delete option
	bool willRemoveFile = false;
	for (int i = 0; i < argc; i++) {
		if (strcmp("d", argv[i]) == 0) {
			willRemoveFile = true;
			break;
		}
	}
	// variables for print status .
	int line_cnt = 0;
	int line_cnt_temp = 0;

	int *step_total_cnt_array = NULL;
	int whole_process = 0;
	int whole_process_current = 0;
	int whole_process_diff = 0;

	// variables for operation 
	int fileIdentifier = 1;
	string basepath = "C:\\Users\\HS-LAB01\\Downloads\\";
	string input_filepath = basepath + "original_test2.txt";
	string output_filepath;
	string buffer_filepath;

	const int data_wholeLine_number = EXSORT::FILEHANDLER::readLine_fromFile(input_filepath);
	if (data_wholeLine_number <= 0) {
		cout << "File open Error" << endl;
		return 0;
	}
	int number_of_files;
	bool is_equal_memory_and_fileLines = false;
	if (data_wholeLine_number%MEMORY_SIZE == 0) {
		number_of_files = data_wholeLine_number / MEMORY_SIZE;
	}
	else {
		number_of_files = data_wholeLine_number / MEMORY_SIZE + 1;
		is_equal_memory_and_fileLines = true;
	}
	const int merge_steps = EXSORT::UTILITY::computeStep(data_wholeLine_number, MEMORY_SIZE);

	// calculate total process .
	step_total_cnt_array = (int*)malloc(sizeof(int) * merge_steps + 1);
	// NUMBEROFWHOLELINE is source file line number .
	step_total_cnt_array[0] = data_wholeLine_number;
	for (int i = 1; i <= merge_steps; i++) {
		//step_total_cnt_array[i] = NUMBEROFWHOLELINE / NUMBEROFCHUNKLINE ;
		step_total_cnt_array[i] = step_total_cnt_array[i - 1] / MEMORY_SIZE;
		if (step_total_cnt_array[i - 1] % MEMORY_SIZE) {
			step_total_cnt_array[i]++;
		}
		printf("INFO: steps[%d] will have %d file(s).\n", i, step_total_cnt_array[i]);
		whole_process += step_total_cnt_array[i];
	}

	printf("INFO: whole_process is %d .\n", whole_process);


	ifstream input_file_descriptor;

	input_file_descriptor.open(input_filepath.c_str());
	for (int file_num = 0; file_num < number_of_files - 1; file_num++) {
		Data step1_data[MEMORY_SIZE];
		ofstream out_file_descriptor;
		EXSORT::FILEHANDLER::fileLines_to_dataArray_as_counts(input_file_descriptor, step1_data, MEMORY_SIZE);
		output_filepath = basepath + "1_" + to_string(file_num + 1) + ".txt";
		EXSORT::UTILITY::quicksort(step1_data, 0, MEMORY_SIZE - 1);
		out_file_descriptor.open(output_filepath.c_str());
		EXSORT::FILEHANDLER::write_data_to_file(out_file_descriptor, step1_data, MEMORY_SIZE);
		out_file_descriptor.close();

		// one file processed .
		//printf("INFO2: whole_process_current == %d .\n", whole_process_current++);
		whole_process_current++;
		whole_process_diff++;
		if (whole_process_diff >= 10) {
			whole_process_diff = 0;
			printf("INFO: whole_process_current == %d .\n", whole_process_current);
		}
	}
	if (is_equal_memory_and_fileLines) {
		Data step1_data[MEMORY_SIZE];
		ofstream out_file_descriptor;
		int remained_number_of_data = data_wholeLine_number % MEMORY_SIZE;
		EXSORT::FILEHANDLER::fileLines_to_dataArray_as_counts(input_file_descriptor, step1_data, remained_number_of_data);
		output_filepath = basepath + "1_" + to_string(number_of_files) + ".txt";
		EXSORT::UTILITY::quicksort(step1_data, 0, remained_number_of_data - 1);
		out_file_descriptor.open(output_filepath.c_str());
		EXSORT::FILEHANDLER::write_data_to_file(out_file_descriptor, step1_data, remained_number_of_data);
		out_file_descriptor.close();
	}
	else {
		Data step1_data[MEMORY_SIZE];
		ofstream out_file_descriptor;
		output_filepath = basepath + "1_" + to_string(number_of_files) + ".txt";
		EXSORT::FILEHANDLER::fileLines_to_dataArray_as_counts(input_file_descriptor, step1_data, MEMORY_SIZE);
		EXSORT::UTILITY::quicksort(step1_data, 0, MEMORY_SIZE - 1);
		out_file_descriptor.open(output_filepath.c_str());
		EXSORT::FILEHANDLER::write_data_to_file(out_file_descriptor, step1_data, MEMORY_SIZE);
		out_file_descriptor.close();
	}

	input_file_descriptor.close();

	//File Sorting and Writing depend upon step

	for (int step = 1; step <= merge_steps; step++) {

		int num_of_previous_files = 0;
		int num_of_next_files;
		if (is_equal_memory_and_fileLines) {
			num_of_next_files = number_of_files / MEMORY_SIZE + 1;
		}
		else {
			num_of_next_files = number_of_files / MEMORY_SIZE;
		}
		int num_of_current_files = number_of_files;
		for (int next_file_number = 0; next_file_number < num_of_next_files; next_file_number++) {

			ifstream buffer_file_descriptor[MEMORY_SIZE];
			Data buffer[MEMORY_SIZE];
			int current_memory_size;
			if (num_of_current_files >= MEMORY_SIZE) {
				current_memory_size = MEMORY_SIZE;
			}
			else if (num_of_current_files < MEMORY_SIZE) {
				current_memory_size = number_of_files % MEMORY_SIZE;
			}
			EXSORT::FILEHANDLER::transfer_fileData_to_dataArray(buffer_file_descriptor, buffer, current_memory_size, num_of_previous_files,
				step, number_of_files, basepath, num_of_current_files);

			EXSORT::UTILITY::quicksort(buffer, 0, current_memory_size - 1);

			ofstream output_file_descriptor;
			output_file_descriptor.open(basepath + to_string(step + 1) + "_" + to_string(next_file_number + 1) + ".txt");

			EXSORT::FILEHANDLER::make_output_file(output_file_descriptor, buffer_file_descriptor, buffer, current_memory_size);
			
			whole_process_current++;
			whole_process_diff++;
			if (whole_process_diff >= 10) {
				whole_process_diff = 0;
				printf("INFO: whole_process_current == %d .\n", whole_process_current);
			}
		}
		if (number_of_files % MEMORY_SIZE == 0) {
			number_of_files = number_of_files / MEMORY_SIZE;
		}
		else {
			number_of_files = number_of_files / MEMORY_SIZE + 1;
		}
	}

	if (willRemoveFile) {
		for (int step = 1; step <=merge_steps; step++) {
			for (int fileNumber = 1; fileNumber <=step_total_cnt_array[step];fileNumber++) {
				string filepath = basepath + to_string(step) + "_" + to_string(fileNumber) + ".txt";
				if (remove(filepath.c_str()) != 0) {
					cout << to_string(step) + "_" + to_string(fileNumber) + ".txt" << " is not removed" << endl;
				}
			}
		}
		cout << "Remove files" << endl;
	}
	return 0;
}
