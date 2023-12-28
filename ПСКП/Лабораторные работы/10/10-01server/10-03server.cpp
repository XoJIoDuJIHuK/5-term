#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

int main() {
    setlocale(LC_ALL, "ru");
    WSADATA data;
    WORD version = MAKEWORD(2, 2);
    int wsOk = WSAStartup(version, &data);
    if (wsOk != 0) {
        std::cerr << "winsock init error" << std::endl;
        return -1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "invalid socket error" << std::endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(3000);
    serverAddress.sin_addr.S_un.S_addr = INADDR_ANY;
    int bindResult = bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
    if (bindResult == SOCKET_ERROR) {
        std::cerr << "socket error" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    char buffer[1024];
    sockaddr_in clientAddress;
    int clientAddressSize = sizeof(clientAddress);
    while (true) {
        ZeroMemory(buffer, sizeof(buffer));
        int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddress, &clientAddressSize);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "client message receiving socket error" << std::endl;
            continue;
        }

        std::string response = "ECHO:" + std::string(buffer, 0, bytesReceived);

        sendto(serverSocket, response.c_str(), response.size() + 1, 0, (sockaddr*)&clientAddress, clientAddressSize);
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}