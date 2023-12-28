﻿#include <iostream>
#include <Windows.h>



int main()
{
	HANDLE semaphore = OpenSemaphoreW(SEMAPHORE_ALL_ACCESS, FALSE, L"OS07_04");

	for (int i = 0; i < 90; i++)
	{
		if (i == 30)
			WaitForSingleObject(semaphore, INFINITE);

		else if (i == 60)
			ReleaseSemaphore(semaphore, 1, NULL);


		printf("[OS07_04B]\t%d\n", i);
		Sleep(100);
	}

	CloseHandle(semaphore);
}