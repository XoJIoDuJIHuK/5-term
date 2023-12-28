#include <iostream>
#include <Windows.h>

int main()
{
    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();

    for (int i = 0; i < 100; i++)
    {
        std::cout << "PID: " << processId << ", TID: " << threadId << std::endl;
        Sleep(1000);
    }

    return 0;
}
