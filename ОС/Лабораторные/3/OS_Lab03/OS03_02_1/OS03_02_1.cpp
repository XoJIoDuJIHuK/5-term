#include <iostream>
#include <Windows.h>

int main() {
    DWORD processId = GetCurrentProcessId();
    for (int i = 0; i < 50; i++) {
        std::cout << "PID: " << processId << std::endl;
        Sleep(1000);
    }
    return 0;
}
