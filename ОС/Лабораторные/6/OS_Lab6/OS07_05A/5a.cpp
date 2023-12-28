#include <iostream>
#include <Windows.h>



int main()
{
	HANDLE event = OpenEventW(EVENT_ALL_ACCESS, FALSE, L"OS07_05");

	WaitForSingleObject(event, INFINITE);
	SetEvent(event);
	for (int i = 1; i <= 90; i++)
	{
		printf("[A]\t %d\n", i);
		Sleep(100);
	}

	CloseHandle(event);
}