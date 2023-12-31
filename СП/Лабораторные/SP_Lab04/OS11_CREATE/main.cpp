#pragma comment(lib, "../x64/debug/OS11_HTAPI.lib")

#include "../OS11_HTAPI/pch.h"
#include "../OS11_HTAPI/HT.h"

using namespace std;

wchar_t* toWCHAR(const char* c);

int main(int argc, char* argv[])
{
	cout << argc << endl;
	if (argc < 6)
	{
		cout << "invalid parameters\n";
		return -1;
	}
	ht::HtHandle* ht = nullptr;
	const char* path = "./../../storage/HTspace.ht";
	try {
		ht = ht::create(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), toWCHAR(path));
	}
	catch (const char* msg) {
		cout <<  msg << endl;
	}
	if (ht)
	{
		cout << "HT-Storage Created ";
		wcout << "filename: " << ht->fileName;
		cout << ", secSnapshotInterval: " << ht->secSnapshotInterval << ", capacity: " << ht->capacity << 
			", maxKeyLength: " << ht->maxKeyLength << ", maxPayloadLength: " << 
			ht->maxPayloadLength << endl;

		ht::close(ht);
	}
	else
		cout << "error while creating" << endl;
}

wchar_t* toWCHAR(const char* c)
{
	wchar_t* wc = new wchar_t[strlen(c) + 1];
	mbstowcs(wc, c, strlen(c) + 1);

	return wc;
}
