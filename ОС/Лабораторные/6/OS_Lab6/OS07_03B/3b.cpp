#include <iostream>
#include <Windows.h>



int main()
{
	HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"OS07_03");

	for (int i = 1; i <= 90; ++i)
	{
		if (i == 30) 
			WaitForSingleObject(mutex, INFINITE);
		
		else if (i == 60) 
			ReleaseMutex(mutex);

		printf("[B]\t%d\n", i);
		Sleep(100);
	}

	CloseHandle(mutex);
}