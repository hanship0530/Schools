#include <iostream>
#include <bitset>

int main() {
	using namespace std;

	const unsigned char opt0 = 1 << 0;
	const unsigned char opt1 = 1 << 1;
	const unsigned char opt2 = 1 << 2;
	const unsigned char opt3 = 1 << 3;

	unsigned char items_flag = 0;

	cout << bitset<8>(opt0) << endl;
	cout << bitset<8>(opt1) << endl;
	cout << bitset<8>(opt2) << endl;
	cout << bitset<8>(opt3) << endl;

	cout << "No item "<<bitset<8>(items_flag)<<endl;

	// item0 on
	items_flag |= opt0;
	cout << "Item0 obtained "<<bitset<8>(items_flag)<<endl;
	// item3 on
	items_flag |= opt3;
	cout << "Item3 obtained "<<bitset<8>(items_flag)<<endl;
	// item3 lost
	items_flag &= ~opt0;
	cout << "Item3 lost "<<bitset<8>(items_flag)<<endl;

	// has item 1?
	if(items_flag & opt1) {cout<<"Has item1"<<endl;}

	items_flag |= (opt2 | opt3);

	cout<<bitset<8>(opt2 | opt3) <<endl;
	cout << "Item2,3 obtained "<<bitset<8>(items_flag)<<endl;

	if((items_flag & opt2) && !(items_flag & opt1)) {
		items_flag ^= opt2;
		items_flag ^= opt1;
		cout<<bitset<8>(items_flag) <<endl;
		cout << "Item1,2 changed "<<bitset<8>(items_flag)<<endl;
	}
	return 0;

}