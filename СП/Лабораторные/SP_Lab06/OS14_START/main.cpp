#pragma warning(disable : 4996)

#include <iostream>
#include <windows.h>
#include <conio.h>

#include "../OS14_HTCOM_LIB/pch.h"
#include "../OS14_HTCOM_LIB/OS14_HTCOM_LIB.h"

#pragma comment(lib, "../x64/Debug/OS14_HTCOM_LIB.lib")

using namespace std;

wchar_t* toWCHAR(const char* c)
{
	wchar_t* wc = new wchar_t[strlen(c) + 1];
	mbstowcs(wc, c, strlen(c) + 1);

	return wc;
}

int main(int argc, char* argv[])
{
	HANDLE hStopEvent = CreateEvent(NULL,
		TRUE, //FALSE - автоматический сброс; TRUE - ручной
		FALSE,
		L"Stop");

	setlocale(LC_ALL, "Russian");

	try
	{
		cout << "Инициализация компонента:" << endl;
		OS14_HTCOM_HANDEL h = OS14_HTCOM::Init();

		ht::HtHandle* ht = nullptr;
		wchar_t* fileName = toWCHAR(argv[1]);

		if (argc == 4)
		{
			wchar_t* username = toWCHAR(argv[2]);
			wchar_t* password = toWCHAR(argv[3]);

			ht = OS14_HTCOM::HT::open(h, fileName, username, password);
		}
		else
		{
			ht = OS14_HTCOM::HT::open(h, fileName);
		}

		if (ht)
		{
			cout << "HT-Storage Start" << endl;
			wcout << "filename: " << ht->fileName << endl;
			cout << "secSnapshotInterval: " << ht->secSnapshotInterval << endl;
			cout << "capacity: " << ht->capacity << endl;
			cout << "maxKeyLength: " << ht->maxKeyLength << endl;
			cout << "maxPayloadLength: " << ht->maxPayloadLength << endl;

			while (!kbhit() && WaitForSingleObject(hStopEvent, 0) == WAIT_TIMEOUT)
				SleepEx(0, TRUE);

			OS14_HTCOM::HT::snap(h, ht);
			OS14_HTCOM::HT::close(h, ht);
		}
		else
			cout << "-- open: error" << endl;

		cout << endl << "Удалить компонент и выгрузить dll, если можно:" << endl;
		OS14_HTCOM::Dispose(h);
	}
	catch (const char* e) { cout << e << endl; }
	catch (int e) { cout << "HRESULT: " << e << endl; }

	system("pause");
}
