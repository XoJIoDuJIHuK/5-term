#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>

using namespace std;

PROCESS_INFORMATION CreateProcessC(WCHAR path[])
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    // Start the child process. 
    if (!CreateProcess(NULL,
        path,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
        )
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return pi;
    }

    return pi;
}

void main()
{
    WCHAR path1[] = L"D:/_УЧЁБА_/5сем/ОС/Лабораторные/3/OS_Lab03/x64/Debug/OS03_02_1.exe";
    WCHAR path2[] = L"D:/_УЧЁБА_/5сем/ОС/Лабораторные/3/OS_Lab03/x64/Debug/OS03_02_2.exe";

    PROCESS_INFORMATION pi1 = CreateProcessC(path1);
    PROCESS_INFORMATION pi2 = CreateProcessC(path2);
    //cockblock

    for (int i = 0; i < 50; i++) {
        cout << "PID: " << GetCurrentProcessId() << endl;
        Sleep(1000);
    }

    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);
    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);
}