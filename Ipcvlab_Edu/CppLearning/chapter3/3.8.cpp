#include <iostream>
#include <bitset>

int main() {
	using namespace std;
	unsigned int a = 3;

	cout<< std::bitset<4>(a) << endl;

	unsigned int b = a << 1;

	cout<< std::bitset<4>(b) << endl;
}