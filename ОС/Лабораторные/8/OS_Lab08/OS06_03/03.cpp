#include <iostream>
#include <iomanip>
#include <Windows.h>
using namespace std;

  /*Т - D2
    о - EE
    ч - F7
    page = 210
    offset = 3823
    page address = D2000
    byte address = D2EEF*/

int main()
{
    const char* l = "Точ";
    char pageNumber = l[0];
    unsigned char xd = (unsigned char)l[1];
    unsigned int offset = xd;
    offset *= 16;
    xd = (unsigned char)l[2] / 16;
    offset += xd;
    xd = (unsigned char)l[0];
    unsigned int pageAddress = xd * 4096;
    uintptr_t byteAddress = pageAddress + offset;
    int pagesNumber = 256;
    int countItems = pagesNumber * 4096 / sizeof(int);
    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    LPVOID allocatedMemoryStart = VirtualAlloc(NULL, pagesNumber * 4096, MEM_COMMIT, PAGE_READWRITE);
    cout << "memory allocated" << endl;
    int* arr = (int*)allocatedMemoryStart;
    for (int i = 0; i < countItems; i++)
        arr[i] = i;
    unsigned char* byte = (unsigned char*)(allocatedMemoryStart) + byteAddress;
    cout << "byte address: " << hex << (uintptr_t)byte << ", byte value: " << hex << 
        *(int*)byte << endl;

    VirtualFree(allocatedMemoryStart, NULL, MEM_RELEASE) ? cout << "virtual memory freed\n" : cout << "virtual memory wasn't freed\n";
}