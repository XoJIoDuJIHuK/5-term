#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
BOOL printFileInfo(LPWSTR fileName) {
    WIN32_FILE_ATTRIBUTE_DATA data;

    if (GetFileAttributesExW(fileName, GetFileExInfoStandard, &data)) {
        SYSTEMTIME st;

        FileTimeToSystemTime(&(data.ftCreationTime), &st);
        wcout << "name: " << fileName << L"\n";
        cout << "type: " << (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? "Directory" : "Regular File") << "\n";
        cout << "size: " << (data.nFileSizeHigh * (MAXDWORD + 1) + data.nFileSizeLow) << " bytes\n";

        printf("created: %02d/%02d/%04d  %02d:%02d:%02d\n", st.wMonth, st.wDay, st.wYear, st.wHour + 3, st.wMinute, st.wSecond);

        FileTimeToSystemTime(&(data.ftLastWriteTime), &st);
        printf("changed: %02d/%02d/%04d   %02d:%02d:%02d\n", st.wMonth, st.wDay, st.wYear, st.wHour + 3, st.wMinute, st.wSecond);

        return true;
    }
    else {
        cout << "Error retrieving file attributes: " << GetLastError() << "\n";

        return false;
    }
}
BOOL printFileTxt(LPWSTR FileName)
{
    HANDLE hFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        cerr << "Failed to open the file: " << FileName << endl;
        return FALSE;
    }

    DWORD fileSize = GetFileSize(hFile, nullptr);
    if (fileSize == INVALID_FILE_SIZE)
    {
        cerr << "Failed to get the file size." << endl;
        CloseHandle(hFile);
        return FALSE;
    }

    char* fileContent = new char[fileSize + 1];
    fileContent[fileSize] = '\0';
    DWORD bytesRead;
    if (!ReadFile(hFile, &fileContent[0], fileSize, &bytesRead, nullptr))
    {
        cerr << "Failed to read the file." << endl;
        CloseHandle(hFile);
        return FALSE;
    }

    wcout << "File Content:" << endl;
    cout << fileContent << endl;
    delete[] fileContent;

    CloseHandle(hFile);
    return TRUE;
}

void main() {
    setlocale(LC_ALL, "ru");
    printFileInfo((LPWSTR)L"D:/_УЧЁБА_/5сем/ОС/Лабораторные/9/OS_Lab09/x64/Debug/xd.txt");
    printFileTxt((LPWSTR)L"D:/_УЧЁБА_/5сем/ОС/Лабораторные/9/OS_Lab09/x64/Debug/xd.txt");
}