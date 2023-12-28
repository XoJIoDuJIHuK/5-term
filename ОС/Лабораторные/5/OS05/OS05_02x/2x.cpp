#include <windows.h>
#include <iostream>
#include <string>

int main() {
    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();

    HANDLE hProcess = GetCurrentProcess();
    DWORD processPriority = GetPriorityClass(hProcess);

    HANDLE hThread = GetCurrentThread();
    int threadPriority = GetThreadPriority(hThread);

    GROUP_AFFINITY groupAffinity;
    GetThreadGroupAffinity(GetCurrentThread(), &groupAffinity);

    std::string s_processPriority;
    std::string s_threadPriority;

    switch (processPriority)
    {
    case IDLE_PRIORITY_CLASS: s_processPriority = "IDLE_PRIORITY_CLASS"; break;
    case BELOW_NORMAL_PRIORITY_CLASS: s_processPriority = "BELOW_NORMAL_PRIORITY_CLASS"; break;
    case NORMAL_PRIORITY_CLASS: s_processPriority = "NORMAL_PRIORITY_CLASS"; break;
    case ABOVE_NORMAL_PRIORITY_CLASS: s_processPriority = "ABOVE_NORMAL_PRIORITY_CLASS"; break;
    case HIGH_PRIORITY_CLASS: s_processPriority = "HIGH_PRIORITY_CLASS"; break;
    case REALTIME_PRIORITY_CLASS: s_processPriority = "REALTIME_PRIORITY_CLASS"; break;
    }
    switch (threadPriority)
    {
    case THREAD_PRIORITY_LOWEST: s_threadPriority = "THREAD_PRIORITY_LOWEST"; break;
    case THREAD_PRIORITY_BELOW_NORMAL: s_threadPriority = "THREAD_PRIORITY_BELOW_NORMAL"; break;
    case THREAD_PRIORITY_NORMAL: s_threadPriority = "THREAD_PRIORITY_NORMAL"; break;
    case THREAD_PRIORITY_ABOVE_NORMAL: s_threadPriority = "THREAD_PRIORITY_ABOVE_NORMAL"; break;
    case THREAD_PRIORITY_HIGHEST: s_threadPriority = "THREAD_PRIORITY_HIGHEST"; break;
    case THREAD_PRIORITY_IDLE: s_threadPriority = "THREAD_PRIORITY_IDLE"; break;
    case THREAD_PRIORITY_TIME_CRITICAL: s_threadPriority = "THREAD_PRIORITY_TIME_CRITICAL"; break;
    }

    for (int i = 0; i < 1000000; i++) {
        if (i % 1000 == 0) {
            Sleep(200);
            std::cout << "iter: " << i << " PID: " << processId << " TID: " << threadId <<
                " process priority: " << s_processPriority << " thread priority: " << s_threadPriority
                << " assigned processor: " << groupAffinity.Group << std::endl;
        }
    }
    getchar();

    return 0;
}
