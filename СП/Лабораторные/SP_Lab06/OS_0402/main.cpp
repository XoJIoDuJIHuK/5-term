#pragma warning(disable : 4996)

#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include "sddl.h"

#include "../OS14_HTCOM_LIB/pch.h"
#include "../OS14_HTCOM_LIB/OS14_HTCOM_LIB.h"

#ifdef _WIN64
#pragma comment(lib, "../x64/Debug/OS14_HTCOM_LIB.lib")
#else
#pragma comment(lib, "../Debug/OS14_HTCOM_LIB.lib")
#endif

using namespace std;


wchar_t* toWCHAR(const char* c)
{
	wchar_t* wc = new wchar_t[strlen(c) + 1];
	mbstowcs(wc, c, strlen(c) + 1);

	return wc;
}

int main(int argc, char* argv[])
{
	wstring eventName = L"Global\\Stop";
	//wstring eventName = (L"Global\\" + std::wstring(L"SP_Lab07_HTService") + L"-event");
	HANDLE hStopEvent = OpenEventW(EVENT_ALL_ACCESS,
		FALSE,
		L"Global\\Stop");
	if (!hStopEvent)
	{
		cout << "event not found " << GetLastError() << endl;
		return -1;
	}
	setlocale(LC_ALL, "ru");

	try
	{
		cout << "Инициализация компонента:" << endl;
		OS14_HTCOM_HANDEL h = OS14_HTCOM::Init();

		ht::HtHandle* ht = NULL;
		if (argc == 2)
		{
			ht = OS14_HTCOM::HT::open(h, toWCHAR(argv[1]), true);
		}
		if (argc == 4)
		{
			ht = OS14_HTCOM::HT::open(h, toWCHAR(argv[1]), toWCHAR(argv[2]), toWCHAR(argv[3]), true);
		}
		if (ht)
			cout << "-- open: success" << endl;
		else
			throw "-- open: error";

		while (WaitForSingleObject(hStopEvent, 0) == WAIT_TIMEOUT) {
			int numberKey = rand() % 50;
			string key = to_string(numberKey);
			cout << key << endl;

			ht::Element* element = OS14_HTCOM::Element::createInsertElement(h, key.c_str(), key.length() + 1, "0", 2);
			if (OS14_HTCOM::HT::insert(h, ht, element))
				cout << "-- insert: success" << endl;
			else
				cout << "-- insert: error" << endl;

			delete element;

			Sleep(1000);
		}
		cout << "timeout\n";

		OS14_HTCOM::HT::close(h, ht);

		cout << endl << "Удалить компонент и выгрузить dll, если можно:" << endl;
		OS14_HTCOM::Dispose(h);
	}
	catch (const char* e) { cout << e << endl; }
	catch (int e) { cout << "HRESULT: " << e << endl; }
}