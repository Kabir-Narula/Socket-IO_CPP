#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "SocketClient.h"
#include <iostream>
#include <cstring> 
#pragma comment(lib, "Ws2_32.lib")

SocketClient::SocketClient() : ClientSocket(INVALID_SOCKET) {}

SocketClient::~SocketClient() {
    Cleanup();
}

bool SocketClient::InitializeWinSock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    return result == NO_ERROR;
}

bool SocketClient::CreateSocket() {
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return ClientSocket != INVALID_SOCKET;
}

bool SocketClient::ConnectToServer(const char* ip, int port) {
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(ip);
    int result = connect(ClientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    return result != SOCKET_ERROR;
}

char* SocketClient::SendAndReceive(const char* message) {
    int sendResult = send(ClientSocket, message, static_cast<int>(strlen(message)), 0);
    if (sendResult == SOCKET_ERROR) {
        std::cerr << "Send failed with error: " << WSAGetLastError() << std::endl;
        return nullptr;
    }

    // Allocate memory for the response, ensure cleanup in case of failure
    char* response = new char[1024];
    int bytesReceived = recv(ClientSocket, response, 1024, 0);
    if (bytesReceived > 0) {
        response[bytesReceived] = '\0'; // Null-terminate the string
        return response;
    }
    else {
        std::cerr << "Receive failed with error: " << WSAGetLastError() << std::endl;
        delete[] response; // Clean up the allocated memory
        return nullptr;
    }
}

void SocketClient::Cleanup() {
    if (ClientSocket != INVALID_SOCKET) {
        closesocket(ClientSocket);
        ClientSocket = INVALID_SOCKET;
    }
    WSACleanup();
}
