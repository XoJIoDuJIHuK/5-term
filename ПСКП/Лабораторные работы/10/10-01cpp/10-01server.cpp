#include <iostream>
#include <cstring>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;
const int BUFFER_SIZE = 1024;
const int SERVER_PORT = 3000;

int main() {
    setlocale(LC_ALL, "ru");
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "winsock init error" << endl;
        return 1;
    }

    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    char buffer[BUFFER_SIZE];

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "invalid socket error" << endl;
        WSACleanup();
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(SERVER_PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        cerr << "socket error" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        cerr << "error while listening" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "server launched at localhost:" << SERVER_PORT << endl;

    while (true) {
        int clientAddressLength = sizeof(clientAddress);

        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "connection with client error" << endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesRead == SOCKET_ERROR) {
            cerr << "reading client message error" << endl;
            closesocket(clientSocket);
            continue;
        }
        string message = string(buffer, bytesRead);
        cout << "got message " << message << endl;
        message = "ECHO:" + message;

        int bytesSent = send(clientSocket, message.c_str(), message.length(), 0);
        if (bytesSent == SOCKET_ERROR) {
            cerr << "error while sending message to client" << endl;
        }

        closesocket(clientSocket);
    }

    closesocket(serverSocket);

    WSACleanup();

    return 0;
}