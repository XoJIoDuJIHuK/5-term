#include <iostream>
#include <ctime>

using namespace std;

void main() {
	long i = 0;
	long start_time = clock();
	bool outputed_5_sec = false;
	bool outputed_10_sec = false;
	while (true) {
		int clocks_passed = clock() - start_time;
		if (clocks_passed / CLOCKS_PER_SEC == 5 && !outputed_5_sec) {
			cout << i << endl;
			outputed_5_sec = true;
		}
		if (clocks_passed / CLOCKS_PER_SEC == 10 && !outputed_10_sec) {
			cout << i << endl;
			outputed_10_sec = true;
		}
		if (clocks_passed / CLOCKS_PER_SEC == 15) {
			break;
		}
		i++;
	}
	cout << i << endl;
};