#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
using namespace std;

HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"OS09");

vector<string> split(string str, string delimiter)
{
    vector<string> result;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != string::npos)
    {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    result.push_back(str.substr(start));

    return result;
}

string toString(LPWSTR lpwstr)
{
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, nullptr, 0, nullptr, nullptr);
    if (bufferSize == 0)
    {
        cerr << "Failed to get the required buffer size for conversion." << endl;
        return "";
    }

    char* buffer = new char[bufferSize];
    if (WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, buffer, bufferSize, nullptr, nullptr) == 0)
    {
        cerr << "Failed to convert LPWSTR to char*." << endl;
        delete[] buffer;
        return "";
    }
    string ret;
    for (int i = 0; i < bufferSize && buffer[i] != '\0'; i++)
    {
        ret += buffer[i];
    }

    return ret;
}

BOOL insRowFileTxt(LPWSTR FileName, LPWSTR str, DWORD row) {
    string stringStr = toString(str);
    if (stringStr == "") {
        return false;
    }
    HANDLE hFile = CreateFileW(FileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
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
    string stringContent;
    for (char c : fileContent) {
        stringContent += c;
    }
    vector<string> lines = split(stringContent, "\r\n");
    lines.insert(lines.begin() + (row == -1 ? lines.size() : row), stringStr);
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
    if (!WriteFile(hFile, updatedContent.c_str(), updatedContent.size() - 2, &bytesWritten, nullptr))
    {
        cerr << "Failed to write to the file." << endl;
        CloseHandle(hFile);
        
        return FALSE;
    }

    CloseHandle(hFile);
    

    return TRUE;
}

void main() {
    setlocale(LC_ALL, "ru");
    insRowFileTxt((LPWSTR)L"D:/_УЧЁБА_/5сем/ОС/Лабораторные/9/OS_Lab09/x64/Debug/xd.txt", (LPWSTR)L"0", 0);
    insRowFileTxt((LPWSTR)L"D:/_УЧЁБА_/5сем/ОС/Лабораторные/9/OS_Lab09/x64/Debug/xd.txt", (LPWSTR)L"-1", -1);
    insRowFileTxt((LPWSTR)L"D:/_УЧЁБА_/5сем/ОС/Лабораторные/9/OS_Lab09/x64/Debug/xd.txt", (LPWSTR)L"5", 5);
    insRowFileTxt((LPWSTR)L"D:/_УЧЁБА_/5сем/ОС/Лабораторные/9/OS_Lab09/x64/Debug/xd.txt", (LPWSTR)L"7", 7);
}