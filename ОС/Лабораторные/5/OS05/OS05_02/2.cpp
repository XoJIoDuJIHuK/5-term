#include <windows.h>
#include <iostream>
#include <sstream>
#include <bitset>

DWORD SwPriority(int i) {
    switch (i) {
    case 1: return IDLE_PRIORITY_CLASS;
    case 2: return BELOW_NORMAL_PRIORITY_CLASS;
    case 3: return NORMAL_PRIORITY_CLASS;
    case 4: return ABOVE_NORMAL_PRIORITY_CLASS;
    case 5: return HIGH_PRIORITY_CLASS;
    case 6: return REALTIME_PRIORITY_CLASS;
    default: throw "No such priority class";
    }
}

PROCESS_INFORMATION CreateProcessC(WCHAR path[], int priority)
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
        CREATE_NEW_CONSOLE | SwPriority(priority),
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

int main(int argc, char* argv[]) {
    //6 4294967295 3 3
    //7 4294967295 2 5
    //8 1 2 5

    if (argc != 4) {
        std::cout << "Wrong parameters number" << std::endl;
        return 1;
    }

    std::istringstream iss1(argv[1]);
    DWORD_PTR processAffinityMask;
    iss1 >> processAffinityMask;

    std::istringstream iss2(argv[2]);
    int P1;
    iss2 >> P1;

    std::istringstream iss3(argv[3]);
    int P2;
    iss3 >> P2;

    std::bitset<12> x(processAffinityMask);
    std::cout << "Process afinity mask: " << x << std::endl;
    std::cout << "P1 priority: " << SwPriority(P1) << std::endl;
    std::cout << "P2 priority: " << SwPriority(P2) << std::endl;

    wchar_t cmdLine[] = L"OS05_02x";
    
    PROCESS_INFORMATION pi1 = CreateProcessC(cmdLine, P1);
    PROCESS_INFORMATION pi2 = CreateProcessC(cmdLine, P2);
    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);
    CloseHandle(pi1.hProcess);
    CloseHandle(pi2.hProcess);
    return 0;
}
