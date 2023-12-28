#include <iostream>
#include <Windows.h>

DWORD WINAPI OS04_02_T1(LPVOID lpParam)
{
    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();
    for (int i = 0; i < 50; i++)
    {
        std::cout << "Thread 1 PID: " << processId << ", TID: " << threadId << std::endl;
        Sleep(1000);
    }
    return 0;
}

DWORD WINAPI OS04_02_T2(LPVOID lpParam)
{
    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();
    for (int i = 0; i < 125; i++)
    {
        std::cout << "Thread 2 PID: " << processId << ", TID: " << threadId << std::endl;
        Sleep(1000);
    }
    return 0;
}

int main()
{
    DWORD processId = GetCurrentProcessId();

    HANDLE hThread1 = CreateThread(NULL, 0, OS04_02_T1, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, OS04_02_T2, NULL, 0, NULL);

    for (int i = 0; i < 100; i++)
    {
        std::cout << "Main thread PID: " << processId << std::endl;
        Sleep(1000);
    }

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    CloseHandle(hThread1);
    CloseHandle(hThread2);

    return 0;
}