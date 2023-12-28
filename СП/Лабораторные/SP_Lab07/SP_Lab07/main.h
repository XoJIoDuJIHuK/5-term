#pragma once
//tcp
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
//
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <tchar.h>
#include <string>
//utf8
#include <locale>
#include <codecvt>
#include "sddl.h"
//sleep
#include <thread>
#include <chrono>

#include "D:/_УЧЁБА_/5сем/СП/Лабораторные/SP_Lab06/OS14_HTCOM_LIB/pch.h"
#include "D:/_УЧЁБА_/5сем/СП/Лабораторные/SP_Lab06/OS14_HTCOM_LIB/OS14_HTCOM_LIB.h"

#define SERVICENAME L"SP_Lab07_HTService"
#define SERVICEPATH L"D:/_УЧЁБА_/5сем/СП/Лабораторные/SP_Lab07/x64/Debug/SP_Lab07.exe"
#define HTPATH L"D:/HTspace.ht"

#define USERNAME L"HTuser01"
#define PASSWORD L"9900"

#define TRACEPATH L"D:/_УЧЁБА_/5сем/СП/Лабораторные/SP_Lab06/storage/service.trace"

#include <conio.h>
#include "D:/_УЧЁБА_/5сем/СП/Лабораторные/SP_Lab06/OS14_HTCOM_LIB/pch.h"
#include "D:/_УЧЁБА_/5сем/СП/Лабораторные/SP_Lab06/OS14_HTCOM_LIB/OS14_HTCOM_LIB.h"
#pragma comment(lib, "D:/_УЧЁБА_/5сем/СП/Лабораторные/SP_Lab06/x64/Debug/OS14_HTCOM_LIB.lib")
#define STORAGEPATH L"D:/403.ht"
#define USERNAME L"HTuser01"
#define PASSWORD L"9900"

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
VOID WINAPI ServiceHandler(DWORD fdwControl);

SECURITY_ATTRIBUTES getSecurityAttributes();
void trace(const char* msg, int r = std::ofstream::app);