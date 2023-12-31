#pragma warning(disable : 4996)

#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <fstream>

#include "../OS13_HTCOM_LIB/pch.h"
#include "../OS13_HTCOM_LIB/OS13_HTCOM_LIB.h"

#pragma comment(lib, "D:/_УЧЁБА_/5сем/СП/Лабораторные/SP_Lab05/lab5/x64/Debug/OS13_HTCOM_LIB.lib")

using namespace std;

int charToInt(char* str)
{
	stringstream convert;
	convert << str;
	int num;
	convert >> num;

	return num;
}

string incrementPayload(char* str)
{
	int oldNumberPayload = charToInt(str);
	int newNumberPayload = oldNumberPayload + 1;
	string newPayload = to_string(newNumberPayload);

	return newPayload;
}

wchar_t* toWCHAR(const char* c)
{
	wchar_t* wc = new wchar_t[strlen(c) + 1];
	mbstowcs(wc, c, strlen(c) + 1);

	return wc;
}

int main(int argc, char* argv[])
{
	if (argc != 2) {
		cout << "invalid parameters" << endl;
		return 1;
	}
	HANDLE hStopEvent = CreateEvent(NULL,
		TRUE, //FALSE - автоматический сброс; TRUE - ручной
		FALSE,
		toWCHAR(argv[1]) + 1);
	setlocale(LC_ALL, "Russian");

	try
	{
		cout << "Инициализация компонента:" << endl;
		OS13_HTCOM_HANDLE h = OS13_HTCOM::Init();
		ht::HtHandle* ht = OS13_HTCOM::HT::open(h, toWCHAR(argv[1]), true);
		if (ht)
			cout << "opened" << endl;
		else
			throw "error whie opening";

		while (WaitForSingleObject(hStopEvent, 0) == WAIT_TIMEOUT) {
			int numberKey = rand() % 50;
			string key = to_string(numberKey);
			cout << key << endl;

			ht::Element* element = OS13_HTCOM::HT::get(h, ht, OS13_HTCOM::Element::createGetElement(h, key.c_str(), key.length() + 1));
			if (element)
			{
				cout << "got" << endl;
				string newPayload = incrementPayload((char*)element->payload);

				if (OS13_HTCOM::HT::update(h, ht, element, newPayload.c_str(), newPayload.length() + 1)){
					cout << "upated" << endl;
				}
				else{
					cout << "error while updating" << endl;
				}
			}	
			else{
				cout << "error while getting" << endl;
			}

			Sleep(1000);
		}

		OS13_HTCOM::HT::close(h, ht);

		cout << endl << "Удалить компонент и выгрузить dll, если можно:" << endl;
		OS13_HTCOM::Dispose(h);
	}
	catch (const char* e) { cout << e << endl; }
	catch (int e) { cout << "HRESULT: " << e << endl; }
	system("pause");
}