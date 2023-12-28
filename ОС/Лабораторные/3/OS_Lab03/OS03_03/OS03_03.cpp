//#include "psapi.h"
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

using namespace std;

void main()
{
    cout << "PID: " << GetCurrentProcessId() << endl;
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hProcessSnap, &pe32)) {
        cout << pe32.th32ProcessID << endl;
        while (Process32Next(hProcessSnap, &pe32)) {
            cout << pe32.th32ProcessID << endl;
        }
    }
    else return;
    // clean the snapshot object
    CloseHandle(hProcessSnap);

    return;
}