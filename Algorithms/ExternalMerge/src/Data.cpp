#pragma once
#include "Data.h"
#include "pch.h"

class Data {
public:
	int id;
	char data[30];
	int price;
	int fileIndex = 0;
	/*
	Data() {
		id = 0;
		price = 0;
		*data = {' ',};
		fileIndex = 0;
	}
	Data(const Data& d) {
		id = d.id;
		*data = *d.data;
		price = d.price;
		fileIndex = d.fileIndex;
	}
	*/
};