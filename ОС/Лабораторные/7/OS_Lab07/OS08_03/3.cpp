#include <iostream>
#include <Windows.h>

using namespace std;

bool toPrint = false;
bool toContinue = true;

DWORD WINAPI printThreadFunction(LPVOID param) {
	HANDLE timerHandle = CreateWaitableTimerW(0, FALSE, L"3SecDelay");
	LARGE_INTEGER Li{};
	int interval = 3;
	Li.QuadPart = -(10000000LL * interval);
	while (toContinue) {
		SetWaitableTimer(timerHandle, &Li, interval * 1000, NULL, NULL, FALSE);
		WaitForSingleObject(timerHandle, INFINITE);
		toPrint = true;
	}
	return 0;
}

DWORD WINAPI breakThreadFunction(LPVOID param) {
	HANDLE timerHandle = CreateWaitableTimerW(0, FALSE, L"15SecDelay");
	LARGE_INTEGER Li{};
	int interval = 15;
	Li.QuadPart = -(10000000LL * interval);
	SetWaitableTimer(timerHandle, &Li, interval * 1000, NULL, NULL, FALSE);
	WaitForSingleObject(timerHandle, INFINITE);
	toContinue = false;
	return 0;
}

int main() {
	HANDLE threads[] = {
		CreateThread(NULL, 0, printThreadFunction, NULL, 0, NULL),
		CreateThread(NULL, 0, breakThreadFunction, NULL, 0, NULL)
	};
	unsigned long long i = 0;
	while (toContinue) {
		if (toPrint) {
			cout << i << endl;
			toPrint = false;
		}
		i++;
	}
	long start_time = clock();
	while ((clock() - start_time) / CLOCKS_PER_SEC < 3)
	{
		if (toPrint)
		{
			cout << i << endl;
			break;
		}
	}
	WaitForMultipleObjects(2, threads, true, INFINITE);
	return 0;
};