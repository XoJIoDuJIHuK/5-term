#pragma comment(lib, "../x64/Debug/OS11_HTAPI.lib")

#include <string>
#include <chrono>
#include <sstream>
#include "../OS11_HTAPI/pch.h"
#include "../OS11_HTAPI/HT.h"

using namespace std;
int charToInt(char* str);
string incrementPayload(char* str);

wchar_t* toWCHAR(const char* c)
{
	wchar_t* wc = new wchar_t[strlen(c) + 1];
	mbstowcs(wc, c, strlen(c) + 1);

	return wc;
}
void printTime() {
	auto currentTime = chrono::system_clock::now();
	auto currentTimeMs = chrono::time_point_cast<chrono::milliseconds>(currentTime);
	auto milliseconds = currentTimeMs.time_since_epoch().count() % 1000;
	time_t currentTimeT = chrono::system_clock::to_time_t(currentTime);
	string timeString = ctime(&currentTimeT);
	cout << timeString.substr(0, timeString.length() - 1) << "." << milliseconds;
}

int main(int argc, char* argv[])
{
	try 
	{
		Sleep(1000);
		ht::HtHandle* ht = ht::open(toWCHAR(argv[1]), true);
		if (ht)
			cout << "successfully opened" << endl;
		else
			throw " failed to open";

		while (true) {
			//srand(time(0));
			int numberKey = rand() % 50;
			string key = to_string(numberKey);
			printTime();
			ht::Element* element = ht::get(ht, new ht::Element(key.c_str(), key.length() + 1));
			if (element)
			{
				cout << " succeffully got element " << key << endl;
				string newPayload = incrementPayload((char*)element->payload);

				if (ht::update(ht, element, newPayload.c_str(), newPayload.length() + 1))
					cout << "succeffully updated payload to " << newPayload << endl;
				else
					cout << "error while updating" << endl;
			}
			else
				cout << " could not get element " << key << endl;

			Sleep(1000);
		}
	}
	catch (const char* msg)
	{
		cout << msg << endl;
	}
}

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
