#pragma warning(disable : 4996)
#include <windows.h>
#include <iostream>
using namespace std;
wchar_t* toWCHAR(const char* c)
{
	wchar_t* wc = new wchar_t[strlen(c) + 1];
	mbstowcs(wc, c, strlen(c) + 1);

	return wc;
}

int main(int argc, char* argv[])
{
	wchar_t* eventName = toWCHAR(argv[1]);
	if (wcscmp(eventName, L"Stop") != 0) {
		eventName += 1;
	}
	for (int i = 0; i < wcslen(eventName); i++) {
		wcout << (int)eventName[i] << L" ";
	}
	cout << endl;
	HANDLE event = CreateEvent(NULL, TRUE, FALSE, eventName);
	std::cout << "emitted event ";
	std::wcout << eventName << std::endl;
	SetEvent(event);
	system("pause");
}

