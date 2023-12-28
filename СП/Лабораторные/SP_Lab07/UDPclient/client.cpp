#include <iostream>
#include "Winsock2.h"
#include "WS2tcpip.h"

#pragma comment(lib, "WS2_32.lib")

using namespace std;

string GetErrorMsgText(int code);
string SetErrorMsgText(string msgText, int code);


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cout << "Invalid agguments number xd\n";
	}
	setlocale(LC_ALL, "rus");
	try {
		WSADATA wsaData;
		SOCKET clientSocket;

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
		}

		if ((clientSocket = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
		{
			throw  SetErrorMsgText("socket:", WSAGetLastError());
		}

		SOCKET serverSocket;

		SOCKADDR_IN serv;                    // параметры  сокета сервера
		serv.sin_family = AF_INET;// используется IP-адресация  
		int port = 2000;
		serv.sin_port = htons(port);                   // TCP-порт 2000
		inet_pton(AF_INET, "127.0.0.1", &(serv.sin_addr)); // адрес сервера

		if ((connect(clientSocket, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
		{
			throw  SetErrorMsgText("connect:", WSAGetLastError());
		}

		string msg = argv[1];
		int  lobuf = 0;                    //количество отправленных байь 
		if ((lobuf = send(clientSocket, msg.c_str(), msg.size() + 1, NULL)) == SOCKET_ERROR)
		{
			throw  SetErrorMsgText("send:", WSAGetLastError());
		}

		if (closesocket(clientSocket) == SOCKET_ERROR)
		{
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		}

		if (WSACleanup() == SOCKET_ERROR)
		{
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
		}
	}
	catch (string errorMsgText) {
		{ cout << endl << "WSAGetLastError: " << errorMsgText; }
	}

	return 0;
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
	default: msgText = "***ERROR***";
		break;
	}
	return msgText;
}


string SetErrorMsgText(string msgText, int code) // Функция возвращает сообщение ошибки
{
	return msgText + GetErrorMsgText(code);
}