#include <iostream>
#include <Windows.h>

using namespace std;

PROCESS_INFORMATION createProcess(LPWSTR path)
{
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInfo;

	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	startupInfo.cb = sizeof(STARTUPINFO);
	if (CreateProcessW(NULL, path, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo, &processInfo))
		cout << "Process created.\n";
	else
		cout << "error by creating process" << endl;

	return processInfo;
}

int main() {
	wchar_t process1minutePath[]  = L"OS08_04_X 1";
	wchar_t process2minutesPath[] = L"OS08_04_X 2";
	HANDLE processes[2];
	processes[0] = createProcess(process1minutePath).hProcess;
	processes[1] = createProcess(process2minutesPath).hProcess;
	WaitForMultipleObjects(2, processes, TRUE, INFINITE);
}