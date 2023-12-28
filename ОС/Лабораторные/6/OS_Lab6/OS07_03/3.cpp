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
	HANDLE mutex;
	HANDLE processes[size];

	wchar_t path1[] = L"OS07_03A";
	wchar_t path2[] = L"OS07_03B";
	processes[0] = createProcess(path1).hProcess;
	processes[1] = createProcess(path2).hProcess;

	mutex = CreateMutexW(NULL, FALSE, L"OS07_03");

	for (int i = 1; i <= 90; ++i)
	{
		if (i == 30) 
			WaitForSingleObject(mutex, INFINITE);
		
		else if (i == 60) 
			ReleaseMutex(mutex);

		printf("[M]\t %d\n", i);
		Sleep(100);
	}


	WaitForMultipleObjects(size, processes, TRUE, INFINITE);
	for (int i = 0; i < size; i++)
		CloseHandle(processes[i]);

	CloseHandle(mutex);
	cout << '\n';
	system("pause");
	return 0;
}
