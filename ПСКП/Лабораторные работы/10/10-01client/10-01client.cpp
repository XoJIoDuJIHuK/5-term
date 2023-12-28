#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

const int BUFFER_SIZE = 1024;

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Winsock init error" << std::endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "invalid socket" << std::endl;
        WSACleanup();
        return 1;
    }

    std::string serverIP = "127.0.0.1";
    int serverPort = 3000;

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());

    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "connection error" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::string message = "xd cpp 1";
    if (send(clientSocket, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
        std::cerr << "send error" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Получение ответа от сервера
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead == SOCKET_ERROR) {
        std::cerr << "response error" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "response|" << buffer << std::endl;

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}