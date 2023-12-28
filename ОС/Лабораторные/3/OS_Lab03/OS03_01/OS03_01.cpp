#include <iostream>
#include <Windows.h>

int main() {
    DWORD processId = GetCurrentProcessId();
    while (true) {
        std::cout << "PID: " << processId << std::endl;
        Sleep(1000);
    }
    return 0;
}
