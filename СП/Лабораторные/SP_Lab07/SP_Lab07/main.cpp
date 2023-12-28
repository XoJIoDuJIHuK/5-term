#include "main.h"

using namespace std;

#pragma region GLOBAL_VARIABLES
CRITICAL_SECTION critical_section;
WCHAR ServiceName[] = SERVICENAME;
SERVICE_STATUS_HANDLE hServiceStatusHandle;
SERVICE_STATUS serviceStatus;
HANDLE stopEvent = NULL;
bool stopService = false;
SOCKET serverSocket;
#pragma endregion

#pragma region PROTOTYPES
string toUTF8(const wchar_t* wstr);
string GetUsernameUTF8();
string GetErrorMsgText(int code);
string SetErrorMsgText(string msgText, int code);
DWORD WINAPI UDPServer(LPVOID lpParam);
DWORD WINAPI startService(LPVOID lpParam);
DWORD WINAPI resetEvent(LPVOID param);
string ptrToString(void* ptr);
bool SetSecurityInfo(HANDLE hm);
#pragma endregion

VOID WINAPI ServiceMain(DWORD dwArgc, LPWSTR* lpszArgv)
{
	InitializeCriticalSection(&critical_section);
	char temp[121];

	serviceStatus.dwServiceType = SERVICE_WIN32;
	serviceStatus.dwCurrentState = SERVICE_START_PENDING;
	serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
	serviceStatus.dwWin32ExitCode = 0;
	serviceStatus.dwServiceSpecificExitCode = 0;
	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;

	if (!(hServiceStatusHandle = RegisterServiceCtrlHandler(ServiceName, ServiceHandler)))
	{
		sprintf_s(temp, "RegisterServiceCtrlHandler failed, error code = %d", GetLastError());
		trace(temp);
	}
	else
	{
		serviceStatus.dwCurrentState = SERVICE_RUNNING;
		serviceStatus.dwCheckPoint = 0;
		serviceStatus.dwWaitHint = 0;

		if (!SetServiceStatus(hServiceStatusHandle, &serviceStatus))
		{
			sprintf_s(temp, "SetServiceStatus failed, error code = %d", GetLastError());
			trace(temp);
		}
		else
		{
			int k = 0;
			wstring eventName = L"Global\\Stop";
			//wstring eventName = L"Global\\" + std::wstring(SERVICENAME) + L"-event";
			stopEvent = CreateEventW(NULL, TRUE, FALSE, L"Global\\Stop");
			if (!stopEvent)
			{
				trace("ServiceMain|could not create event");
				return;
			}
			if (!SetSecurityInfo(stopEvent))
			{
				trace("ServiceMain|could not set security attributes");
				return;
			}
			ResetEvent(stopEvent);
			trace("Start service", std::ofstream::out);
			trace(("stopEventPtr|" + ptrToString(stopEvent) + "|name|" + 
				string(eventName.begin(), eventName.end())).c_str());
			HANDLE hStart = CreateThread(NULL, 0, startService, NULL, 0, NULL);
			while (serviceStatus.dwCurrentState == SERVICE_RUNNING)
			{
				Sleep(3000);
				sprintf_s(temp, "service running = %d", k++);
				trace(temp);
			}
			WaitForSingleObject(hStart, INFINITE);
		}
	}
}

VOID WINAPI ServiceHandler(DWORD fdwControl)
{
	char temp[121];

	switch (fdwControl)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
	{
		serviceStatus.dwWin32ExitCode = 0;
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		serviceStatus.dwCheckPoint = 0;
		serviceStatus.dwWaitHint = 0;
		SetEvent(stopEvent);
		if (serverSocket)
		{
			closesocket(serverSocket);
		}
		trace("Stop service");

		break;
	}
	case SERVICE_CONTROL_PAUSE:
		serviceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:
		serviceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	default:
		sprintf_s(temp, "Unrecognized opcode %d", fdwControl);
		trace(temp);
	};

	if (!SetServiceStatus(hServiceStatusHandle, &serviceStatus))
	{
		sprintf_s(temp, "SetServiceStatus failed, error code = %d", GetLastError());
		trace(temp);
	}
}

SECURITY_ATTRIBUTES getSecurityAttributes()
{
	const wchar_t* sdd = L"D:"
		L"(D;OICI;GA;;;BG)" //Deny guests
		L"(D;OICI;GA;;;AN)" //Deny anonymous
		L"(A;OICI;GA;;;AU)" //Allow read, write and execute for Users
		L"(A;OICI;GA;;;BA)"; //Allow all for Administrators
	SECURITY_ATTRIBUTES SA;
	ZeroMemory(&SA, sizeof(SA));
	SA.nLength = sizeof(SA);
	ConvertStringSecurityDescriptorToSecurityDescriptor(
		sdd,
		SDDL_REVISION_1,
		&SA.lpSecurityDescriptor,
		NULL);

	return SA;
}

int main()
{
	/*SC_HANDLE schService = NULL, schSManager = NULL;
	try
	{
		schSManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
		if (!schSManager)
		{
			DWORD code = GetLastError();
			throw "schSManager is null";
		}
		CreateServiceW(
			schSManager,
			SERVICENAME,
			SERVICENAME,
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS,
			SERVICE_AUTO_START,
			SERVICE_ERROR_NORMAL,
			SERVICEPATH,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL
		);
		if (!schService)
		{
			DWORD code = GetLastError();
			throw "schService is null";
		}
	}
	catch (char* txt)
	{ 
		cout << txt << endl;
	}
	if (schSManager)
	{
		CloseServiceHandle(schSManager);
	}
	if (schService)
	{
		CloseServiceHandle(schService);
	}*/

	WCHAR ServiceName[] = SERVICENAME;
	SERVICE_TABLE_ENTRY ServiceTableStart[] = {
		{ServiceName, ServiceMain},
		{NULL, NULL}
	};
	if (!StartServiceCtrlDispatcherW(ServiceTableStart))
	{
		trace("StartServiceCtrlDispatcher", ofstream::out);
	}
}


void trace(const char* msg, int r)
{
	EnterCriticalSection(&critical_section);
	std::ofstream out;
	out.open(TRACEPATH, r);
	out << msg << std::endl;
	out.flush();
	out.close();
	LeaveCriticalSection(&critical_section);
}
string toUTF8(const wchar_t* wstr)
{
	wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(wstr);
}
string GetUsernameUTF8()
{
	wchar_t username[UNLEN + 1];
	DWORD usernameLen = UNLEN + 1;
	GetUserNameW(username, &usernameLen);
	return toUTF8(username);
}
string GetErrorMsgText(int code)
{
	string msgText;
	switch (code)
	{
	case WSAEINTR: msgText = "Работа функции прервана";
		break;
	case WSAEACCES: msgText = "Разрешение отвергнуто";
		break;
	case WSAEFAULT: msgText = "Ошибочный адрес";
		break;
	case WSASYSCALLFAILURE: msgText = "Аварийное завершение системного вызова";
		break;
	default: msgText = "***ERROR***|code=" + to_string(code);
		break;
	}
	return msgText;
}
string SetErrorMsgText(string msgText, int code)
{
	return msgText + GetErrorMsgText(code);
}
DWORD WINAPI UDPServer(LPVOID lpParam)
{
	try {
		WSADATA wsaData;

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
		}

		if ((serverSocket = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
		{
			throw  SetErrorMsgText("socket:", WSAGetLastError());
		}

		SOCKADDR_IN serv;				   // параметры сокета sS
		serv.sin_family = AF_INET;		   // используется IP-адресация
		int port = 2000;
		serv.sin_port = htons(port);
		serv.sin_addr.s_addr = INADDR_ANY;
		trace("before bind");
		if (bind(serverSocket, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR) // установка параметров serv 
			//для сокета serverSocket
		{
			throw  SetErrorMsgText("bind:", WSAGetLastError());
		}
		trace("after bind");


		int queueLength = 1;
		if (SOCKET_ERROR == listen(serverSocket, queueLength))
		{
			throw ("Listen SOCKET_ERROR code: " + to_string(WSAGetLastError()));
		}
		trace(toUTF8(L"Сервер начал прослушку").c_str());

		SOCKET clientSocket;	                 // сокет для обмена данными с клиентом 
		SOCKADDR_IN clnt;						 // параметры  сокета клиента
		memset(&clnt, 0, sizeof(clnt));
		int lclnt = sizeof(clnt);			     // размер SOCKADDR_IN

		while (true) {
			if ((clientSocket = accept(serverSocket, (sockaddr*)&clnt, &lclnt)) == INVALID_SOCKET)
			{
				throw  SetErrorMsgText("accept:", WSAGetLastError());
			}

			char ibuf[4096];
			memset(ibuf, 0, sizeof(ibuf));
			int  libuf = 0;
			if ((libuf = recv(clientSocket, ibuf, sizeof(ibuf) - 1, NULL)) == SOCKET_ERROR)
			{
				trace(("Recv: " + to_string(WSAGetLastError())).c_str());
				break;
			}
			else
			{
				string msg = string(ibuf);
				trace(("Got TCP message|" + msg).c_str());
				if (msg == "Stop service" && !stopService)
				{
					trace("Received command to stop service");
					trace(("Setting event " + ptrToString(stopEvent)).c_str());
					SetEvent(stopEvent);
					stopService = true;
				}
				else if (msg == "Resume service" && stopService)
				{
					trace("Received command to resume service");
					stopService = false;
				}
				else
				{
					trace(("Received unknown command|" + msg).c_str());
				}
			}
		}
		if (closesocket(serverSocket) == SOCKET_ERROR)
		{
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		}

		if (WSACleanup() == SOCKET_ERROR)
		{
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
		}


	}
	catch (string errorMsgText) {
		{ trace(("WSAGetLastError: " + errorMsgText).c_str()); }
	}
	return 0;
}
DWORD WINAPI startService(LPVOID lpParam)
{
	setlocale(LC_ALL, "Russian");

	trace(("Current user name: " + GetUsernameUTF8()).c_str());
	CreateThread(NULL, 0, UDPServer, NULL, 0, NULL);

	setlocale(LC_ALL, "Russian");

	try
	{
		trace(toUTF8(L"Инициализация компонента:").c_str());
		OS14_HTCOM_HANDEL h = OS14_HTCOM::Init();

		ht::HtHandle* ht = nullptr;
		const wchar_t* fileName = STORAGEPATH;
		

		while (true)
		{
			if (!stopService)
			{
				ht = OS14_HTCOM::HT::open(h, fileName, USERNAME, PASSWORD);
				if (ht)
				{
					trace("HT-Storage Start");
					trace(("filename: " + toUTF8(ht->fileName)).c_str());
					trace(("secSnapshotInterval: " + to_string(ht->secSnapshotInterval)).c_str());
					trace(("capacity: " + to_string(ht->capacity)).c_str());
					trace(("maxKeyLength: " + to_string(ht->maxKeyLength)).c_str());
					trace(("maxPayloadLength: " + to_string(ht->maxPayloadLength)).c_str());

					while (!stopService)
					{
						SleepEx(0, TRUE);
					}
					DWORD delay = 1;
					trace(("resetting event|" + to_string(clock())).c_str());
					HANDLE resetThread = CreateThread(NULL, 0, resetEvent, &delay, 0, NULL);
					WaitForSingleObject(resetThread, INFINITE);
					trace(("finished resetting event|" + to_string(clock())).c_str());
					OS14_HTCOM::HT::snap(h, ht);
					OS14_HTCOM::HT::close(h, ht);
				}
				else
				{
					trace("-- open: error");
				}
			}
			SleepEx(1000, TRUE);
		}

		trace(toUTF8(L"Удалить компонент и выгрузить dll, если можно:").c_str());
		OS14_HTCOM::Dispose(h);
	}
	catch (const char* e) { trace(e); }
	catch (int e) { trace(("HRESULT: " + to_string(e)).c_str()); }
	return 0;
}
string ptrToString(void* ptr)
{
	stringstream stream;
	stream << "0x" << setfill('0') << setw(sizeof(void*) * 2) << std::hex
		<< reinterpret_cast<uintptr_t>(ptr);
	return stream.str();
}
DWORD WINAPI resetEvent(LPVOID param)
{
	DWORD delay = (DWORD)param;
	if (delay > 10)
	{
		delay = 1;
	}
	Sleep(delay * 1000);
	ResetEvent(stopEvent);
	return 0;
}
bool SetSecurityInfo(HANDLE hm)
{
	const wchar_t* sdd = L"D:"
		L"(D;OICI;GA;;;BG)" //Deny guests
		L"(D;OICI;GA;;;AN)" //Deny anonymous
		L"(A;OICI;GA;;;AU)" //Allow read, write and execute for Users
		L"(A;OICI;GA;;;BA)"; //Allow all for Administrators
	PSECURITY_DESCRIPTOR pSD;
	if (!ConvertStringSecurityDescriptorToSecurityDescriptorW(sdd, SDDL_REVISION_1, &pSD, NULL))
	{
		DWORD error = GetLastError();
		CloseHandle(hm);
		trace("ConvertStringSecurityDescriptorToSecurityDescriptorW|returned false");
		trace(("last error|" + to_string(error)).c_str());
		return false;
	}

	if (!SetKernelObjectSecurity(hm, DACL_SECURITY_INFORMATION, pSD))
	{
		DWORD error = GetLastError();
		LocalFree(pSD);
		CloseHandle(hm);
		trace("SetSecurityInfo|returned false");
		trace(("last error|" + to_string(error)).c_str());
		return false;
	}

	LocalFree(pSD);
	return true;
}