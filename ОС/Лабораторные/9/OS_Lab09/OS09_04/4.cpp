#include <iostream>
#include <vector>
#include <Windows.h>
#include "Shlwapi.h"
#pragma comment(lib, "ShLwApi.lib")
using namespace std;

HANDLE mutex = CreateMutexW(NULL, FALSE, L"OS09");


DWORD getRowsCount(LPCWSTR FileName)
{
    HANDLE hFile = CreateFileW(FileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr,
        OPEN_EXISTING, NULL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        cerr << "Failed to open the file" << endl;
        return FALSE;
    }

    DWORD fileSize = GetFileSize(hFile, nullptr);
    if (fileSize == INVALID_FILE_SIZE)
    {
        cerr << "Failed to get the file size." << endl;
        CloseHandle(hFile);
        return FALSE;
    }

    vector<char> fileContent(fileSize);
    DWORD bytesRead;
    if (!ReadFile(hFile, fileContent.data(), fileSize, &bytesRead, nullptr))
    {
        cerr << "Failed to read the file." << endl;
        CloseHandle(hFile);
        return FALSE;
    }

    CloseHandle(hFile);

    vector<string> lines;
    string currentLine;
    for (char c : fileContent)
    {
        if (c == '\n')
        {
            lines.push_back(currentLine);
            currentLine.clear();
        }
        else
        {
            currentLine += c;
        }
    }
    return lines.size() + 1;
}

BOOL printWatchRowFileTxt(LPWSTR FileName, DWORD mSec)
{
    wstring wFilePath = wstring(FileName);
    DWORD found = wFilePath.find_last_of(L"\\");
    wstring wDirPath = wFilePath.substr(0, found);
    wstring wFileName = wFilePath.substr(found + 1);

    cout << "Watching changes in directory" << endl;
    DWORD initialLinesCount = getRowsCount(FileName);

    HANDLE hDir = FindFirstChangeNotificationW(wDirPath.c_str(), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE);
    if (hDir == INVALID_HANDLE_VALUE)
    {
        cout << "Cannot open directory" << endl;
        return FALSE;
    }
    DWORD initialRowsCount = getRowsCount(FileName);
    DWORD start = clock();
    DWORD xd = (start - (DWORD)clock()) * 1000 / CLOCKS_PER_SEC;
    while (((DWORD)clock() - start) * 1000 / CLOCKS_PER_SEC < mSec)
    {
        if (WaitForSingleObject(hDir, 0) == WAIT_OBJECT_0)
        {
            DWORD rowsCount = getRowsCount(FileName);
            if (rowsCount != initialRowsCount)
            {
                cout << "rows count changed to " << rowsCount << endl;
                initialRowsCount = rowsCount;
            }
            FindNextChangeNotification(hDir);
        }
    }
    FindCloseChangeNotification(hDir);
    return TRUE;
}

void main()
{
    setlocale(LC_ALL, "ru");
    printWatchRowFileTxt((LPWSTR)L"D:\\_УЧЁБА_\\5сем\\ОС\\Лабораторные\\9\\OS_Lab09\\x64\\Debug\\xd.txt", 10000);
}