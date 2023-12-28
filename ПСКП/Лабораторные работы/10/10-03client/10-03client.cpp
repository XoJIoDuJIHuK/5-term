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

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "invalid socket" << std::endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(3000);
    inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr));

    std::string message = "xd 3 cpp";
    int sendResult = sendto(clientSocket, message.c_str(), message.size() + 1, 0, (sockaddr*)&serverAddress, sizeof(serverAddress));
    if (sendResult == SOCKET_ERROR) {
        std::cerr << "send error" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    char buffer[1024];
    sockaddr_in serverResponseAddress;
    int serverResponseAddressSize = sizeof(serverResponseAddress);
    ZeroMemory(buffer, sizeof(buffer));
    int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (sockaddr*)&serverResponseAddress, &serverResponseAddressSize);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "get error" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "response|" << buffer << std::endl;

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}