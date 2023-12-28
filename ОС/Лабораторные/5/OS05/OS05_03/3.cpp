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

DWORD SwPriorityThread(int i) {
    switch (i) {
    case 1: return THREAD_PRIORITY_LOWEST;
    case 2: return THREAD_PRIORITY_BELOW_NORMAL;
    case 3: return THREAD_PRIORITY_NORMAL;
    case 4: return THREAD_PRIORITY_ABOVE_NORMAL;
    case 5: return THREAD_PRIORITY_HIGHEST;
    case 6: return THREAD_PRIORITY_IDLE;
    default: throw "No such priority class";
    }
}

DWORD WINAPI TA(LPVOID lpParam) {
    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();

    HANDLE hProcess = GetCurrentProcess();
    DWORD processPriority = GetPriorityClass(hProcess);

    HANDLE hThread = GetCurrentThread();
    int threadPriority = GetThreadPriority(hThread);

    GROUP_AFFINITY groupAffinity;
    GetThreadGroupAffinity(GetCurrentThread(), &groupAffinity);

    for (int i = 0; i < 1000000; i++) {
        if (i % 1000 == 0) {
            Sleep(200);
            std::cout << "iter: " << i << "  PID: " << GetCurrentProcessId() << "  TID: " << GetCurrentThreadId();
            std::cout << " Process priority: ";
            switch (GetPriorityClass(hProcess))
            {
            case IDLE_PRIORITY_CLASS: std::cout << "IDLE_PRIORITY_CLASS"; break;
            case BELOW_NORMAL_PRIORITY_CLASS: std::cout << "BELOW_NORMAL_PRIORITY_CLASS"; break;
            case NORMAL_PRIORITY_CLASS: std::cout << "NORMAL_PRIORITY_CLASS"; break;
            case ABOVE_NORMAL_PRIORITY_CLASS: std::cout << "ABOVE_NORMAL_PRIORITY_CLASS"; break;
            case HIGH_PRIORITY_CLASS: std::cout << "HIGH_PRIORITY_CLASS"; break;
            case REALTIME_PRIORITY_CLASS: std::cout << "REALTIME_PRIORITY_CLASS"; break;
            }
            std::cout << " Thread priority: ";
            switch (GetThreadPriority(hThread))
            {
            case THREAD_PRIORITY_LOWEST: std::cout << "THREAD_PRIORITY_LOWEST"; break;
            case THREAD_PRIORITY_BELOW_NORMAL: std::cout << "THREAD_PRIORITY_BELOW_NORMAL"; break;
            case THREAD_PRIORITY_NORMAL: std::cout << "THREAD_PRIORITY_NORMAL"; break;
            case THREAD_PRIORITY_ABOVE_NORMAL: std::cout << "THREAD_PRIORITY_ABOVE_NORMAL"; break;
            case THREAD_PRIORITY_HIGHEST: std::cout << "THREAD_PRIORITY_HIGHEST"; break;
            case THREAD_PRIORITY_IDLE: std::cout << "THREAD_PRIORITY_IDLE"; break;
            case THREAD_PRIORITY_TIME_CRITICAL: std::cout << "THREAD_PRIORITY_TIME_CRITICAL"; break;
            }
            std::cout << " assigned processor: " << groupAffinity.Group << std::endl;
        }
    }

    return 0;
}

int main(int argc, char* argv[]) {
    //13 4294967295 3 3 3
    //14 4294967295 3 1 5
    //15 1 3 1 5

    if (argc != 5) {
        std::cout << "Wrong parameters number!" << std::endl;
        return 1;
    }

    std::istringstream iss1(argv[1]);
    DWORD_PTR P1;
    iss1 >> P1;
    std::bitset<8> x(P1);

    std::istringstream iss2(argv[2]);
    int P2;
    iss2 >> P2;

    std::istringstream iss3(argv[3]);
    int P3;
    iss3 >> P3;

    std::istringstream iss4(argv[4]);
    int P4;
    iss4 >> P4;

    std::cout << "Process affinity mask: " << x << std::endl;
    std::cout << "Process priority: " << SwPriority(P2) << std::endl;
    std::cout << "T1 priority: " << SwPriorityThread(P3) << std::endl;
    std::cout << "T2 priority: " << SwPriorityThread(P4) << std::endl;

    SetPriorityClass(GetCurrentProcess(), SwPriority(P2));

    HANDLE hThread1 = CreateThread(NULL, 0, TA, NULL, 0, NULL);
    SetThreadPriority(hThread1, SwPriorityThread(P3));

    HANDLE hThread2 = CreateThread(NULL, 0, TA, NULL, 0, NULL);
    SetThreadPriority(hThread2, SwPriorityThread(P4));

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    return 0;
}
