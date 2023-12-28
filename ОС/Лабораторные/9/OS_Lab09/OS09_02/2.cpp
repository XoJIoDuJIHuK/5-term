#include <iostream>
#include <vector>
#include <Windows.h>
using namespace std;

HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"OS09");

BOOL delRowFileTxt(LPWSTR FileName, DWORD row)
{
    HANDLE hFile = CreateFile(FileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
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
        if (c == '\n' || c == '\r')
        {
            if (!currentLine.empty())
            {
                lines.push_back(currentLine);
                currentLine.clear();
            }
        }
        else
        {
            currentLine += c;
        }
    }

    if (!currentLine.empty())
    {
        lines.push_back(currentLine);
    }

    if (row <= 0 || row > lines.size())
    {
        cerr << "No such row" << endl;
        return FALSE;
    }

    lines.erase(lines.begin() + row - 1);

    string updatedContent;
    for (const auto& line : lines)
    {
        updatedContent += line + "\r\n";
    }

    hFile = CreateFile(FileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        cerr << "Failed to open the file for writing: " << FileName << endl;
        return FALSE;
    }

    DWORD bytesWritten;
    if (!WriteFile(hFile, updatedContent.c_str(), updatedContent.size(), &bytesWritten, nullptr))
    {
        cerr << "Failed to write to the file." << endl;
        CloseHandle(hFile);
        return FALSE;
    }

    CloseHandle(hFile);

    return TRUE;
}
BOOL printFileTxt(LPWSTR FileName)
{
    HANDLE hFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
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
    delRowFileTxt((LPWSTR)L"D:/_УЧЁБА_/5сем/ОС/Лабораторные/9/OS_Lab09/x64/Debug/xd.txt", 1);
    delRowFileTxt((LPWSTR)L"D:/_УЧЁБА_/5сем/ОС/Лабораторные/9/OS_Lab09/x64/Debug/xd.txt", 3);
    delRowFileTxt((LPWSTR)L"D:/_УЧЁБА_/5сем/ОС/Лабораторные/9/OS_Lab09/x64/Debug/xd.txt", 8);
    delRowFileTxt((LPWSTR)L"D:/_УЧЁБА_/5сем/ОС/Лабораторные/9/OS_Lab09/x64/Debug/xd.txt", 10);
    printFileTxt((LPWSTR)L"D:/_УЧЁБА_/5сем/ОС/Лабораторные/9/OS_Lab09/x64/Debug/xd.txt");
}