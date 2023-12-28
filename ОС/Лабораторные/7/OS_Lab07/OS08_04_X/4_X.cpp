#include <iostream>
#include <string>
#include <ctime>

using namespace std;

int lastPrimeNumber = 0;
bool toContinue = true;

bool isPrime(int number) {
	int root = (int)sqrt(number);
	for (int i = 2; i <= root; i++) {
		if (((double)number / (double)i) == (number / i)) {
			return false;
		}
	}
	return true;
}

int getNextPrimeNumber() {
	do {
		lastPrimeNumber++;
	} while (!isPrime(lastPrimeNumber));
	return lastPrimeNumber;
}

int main(int argc, char* argv[]) {
	int minutesToLive = 0;
	if (argc > 1) {
		minutesToLive = stoi(argv[1]);
	}
	else {
		cout << "No command line parameter received." << endl;
		return -1;
	}
	int primeNumberNumber = 0;
	long startTime = clock();
	while ((clock() / CLOCKS_PER_SEC / 60) < minutesToLive) {
		cout << primeNumberNumber++ << ": " << getNextPrimeNumber() << endl;
	}
	cout << clock() - startTime << endl;
	getchar();
	return 0;
}