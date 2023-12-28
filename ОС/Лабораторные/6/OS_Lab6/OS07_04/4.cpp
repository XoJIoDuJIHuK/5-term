#include <iostream>
#include <Windows.h>
using namespace std;





PROCESS_INFORMATION createProcess(WCHAR path[])
{
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInfo;

	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	startupInfo.cb = sizeof(STARTUPINFO);

	if (CreateProcessW(NULL, path, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo, &processInfo))
		cout << "Process created\n";
	else
		cout << "error by creating process" << endl;

	return processInfo;
}





int main()
{
	const int size = 2;
	HANDLE semaphore;
	HANDLE processes[size];

	wchar_t path1[] = L"OS07_04A";
	wchar_t path2[] = L"OS07_04B";
	processes[0] = createProcess(path1).hProcess;
	processes[1] = createProcess(path2).hProcess;

	semaphore = CreateSemaphoreW(NULL, 2, 2, L"OS07_04");

	for (int i = 1; i <= 90; i++)
	{
		if (i == 30) 
			WaitForSingleObject(semaphore, INFINITE);

		else if (i == 60) 
			ReleaseSemaphore(semaphore, 1, NULL);

		printf("[OS07_04]\t %d\n", i);
		Sleep(100);
	}

	WaitForMultipleObjects(size, processes, TRUE, INFINITE);
	for (int i = 0; i < size; i++)
		CloseHandle(processes[i]);

	CloseHandle(semaphore);
	cout << '\n';
	system("pause");
	return 0;
}
