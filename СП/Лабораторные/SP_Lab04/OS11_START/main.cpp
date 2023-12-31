#pragma comment(lib, "../x64/debug/OS11_HTAPI.lib")

#include <conio.h>
#include "../OS11_HTAPI/pch.h"
#include "../OS11_HTAPI/HT.h"

using namespace std;

wchar_t* toWCHAR(const char* c);

int main(int argc, char* argv[])
{
	cout << clock() << endl;
	ht::HtHandle* ht = nullptr;

	wchar_t* fileName = toWCHAR(argv[1]);
	ht = ht::open(fileName);
	if (ht)
	{
		cout << "HT-Storage Start ";
		wcout << "filename: " << ht->fileName;
		cout << ", secSnapshotInterval: " << ht->secSnapshotInterval << ", capacity: " << ht->capacity
			<< endl << ", maxKeyLength: " << ht->maxKeyLength << ", maxPayloadLength: " << ht->maxPayloadLength
			<< endl;

		while (!kbhit())
			SleepEx(10, TRUE);

		ht::close(ht);
	}
	else
		cout << "error while openenig" << endl;
}

wchar_t* toWCHAR(const char* c)
{
	wchar_t* wc = new wchar_t[strlen(c) + 1];
	mbstowcs(wc, c, strlen(c) + 1);

	return wc;
}
