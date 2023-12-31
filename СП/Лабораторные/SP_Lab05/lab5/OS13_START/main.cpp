#pragma warning(disable : 4996)

#include <iostream>
#include <windows.h>
#include <conio.h>

#include "../OS13_HTCOM_LIB/pch.h"
#include "../OS13_HTCOM_LIB/OS13_HTCOM_LIB.h"

#pragma comment(lib, "D:/_УЧЁБА_/5сем/СП/Лабораторные/SP_Lab05/lab5/x64/Debug/OS13_HTCOM_LIB.lib")

using namespace std;

wchar_t* toWCHAR(const char* c)
{
	wchar_t* wc = new wchar_t[strlen(c) + 1];
	mbstowcs(wc, c, strlen(c) + 1);

	return wc;
}

int main(int argc, char* argv[])
{
	wchar_t* fileName = toWCHAR(argv[1]);
	wchar_t* eventName = toWCHAR(argv[1]) + 1;
	wcout << L"START " << fileName << L"\n";
	HANDLE hStopEvent = CreateEventW(NULL,
		TRUE, //FALSE - автоматический сброс; TRUE - ручной
		FALSE,
		eventName);

	setlocale(LC_ALL, "Russian");

	try
	{
		cout << "Инициализация компонента:" << endl;
		OS13_HTCOM_HANDLE h = OS13_HTCOM::Init();

		ht::HtHandle* ht = nullptr;
		ht = OS13_HTCOM::HT::open(h, fileName, false);
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

			OS13_HTCOM::HT::snap(h, ht);
			OS13_HTCOM::HT::close(h, ht);
		}
		else
			cout << "error whie opening" << endl;

		cout << endl << "Удалить компонент и выгрузить dll, если можно:" << endl;
		OS13_HTCOM::Dispose(h);
	}
	catch (const char* e) { cout << e << endl; }
	catch (int e) { cout << "HRESULT: " << e << endl; }
	system("pause");
}
