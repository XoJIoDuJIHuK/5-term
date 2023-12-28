#include <windows.h>
#include <iostream>
#include <clocale>
#include <bitset>
#include <intrin.h>

int main() {
    setlocale(LC_ALL, "ru");

    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();

    HANDLE hProcess = GetCurrentProcess();
    DWORD processPriority = GetPriorityClass(hProcess);

    HANDLE hThread = GetCurrentThread();
    int threadPriority = GetThreadPriority(hThread);

    DWORD_PTR processAffinityMask;
    DWORD_PTR systemAffinityMask;

    GetProcessAffinityMask(hProcess, &processAffinityMask, &systemAffinityMask);
    std::bitset<12> x(processAffinityMask);

    int amountOProcessors = 0;
    while (processAffinityMask)
    {
        amountOProcessors += processAffinityMask & 1;
        processAffinityMask >>= 1;
    }


    GROUP_AFFINITY groupAffinity;
    GetThreadGroupAffinity(GetCurrentThread(), &groupAffinity);

    std::cout << "Идентификатор текущего процесса: " << processId << std::endl;
    std::cout << "Идентификатор текущего потока: " << threadId << std::endl;
    std::cout << "Приоритет текущего процесса: " << processPriority << std::endl;
    std::cout << "Приоритет текущего потока: " << threadPriority << std::endl;
    std::cout << "Маска доступных процессу процессоров: " << x << std::endl;
    std::cout << "Количество доступных процессу процессоров: " << amountOProcessors << std::endl;
    std::cout << "Процессор, назначенный текущему потоку: " << groupAffinity.Group << std::endl;

    return 0;
}
