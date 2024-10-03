#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "SocketServer.h"
#include <iostream>
#include <cstring> 
#pragma comment(lib, "Ws2_32.lib")

SocketServer::SocketServer() : ServerSocket(INVALID_SOCKET) {}

SocketServer::~SocketServer() {
    Cleanup();
}

bool SocketServer::InitializeWinSock() {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

bool SocketServer::CreateSocket() {
    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return ServerSocket != INVALID_SOCKET;
}

bool SocketServer::BindSocket(int port) {
    sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_addr.s_addr = INADDR_ANY;
    SvrAddr.sin_port = htons(port);
    return bind(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) != SOCKET_ERROR;
}

bool SocketServer::ListenForClients() {
    return listen(ServerSocket, 1) != SOCKET_ERROR;
}

bool SocketServer::AcceptClient() {
    SOCKET ClientSocket = accept(ServerSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        return false;
    }
    std::cout << "Connection Established\n";
    HandleClient(ClientSocket);
    closesocket(ClientSocket);
    return true;
}

void SocketServer::HandleClient(SOCKET clientSocket) {
    char RxBuffer[128] = { 0 };
    int bytesReceived = recv(clientSocket, RxBuffer, sizeof(RxBuffer) - 1, 0); // Prevent buffer overrun
    if (bytesReceived > 0) {
        RxBuffer[bytesReceived] = '\0'; // Null-terminate the string

        
        Email emails[] = {
            Email("JobsS", EmailDetails("Bill", "Jobs", "Finance Presentation", "Looking forward to our meeting on finance")),
            Email("BillG", EmailDetails("Warren", "Bill", "Dinner", "It was lovely meeting you over dinner")),
            Email("WarrenB", EmailDetails("Elon", "Warren", "SpaceX", "How's new SpaceX project going?")),
            Email("JobsS", EmailDetails("Elon", "Jobs", "Twitter", "How is it going at Twitter?"))
        };

        bool found = false;
        std::string response;
        for (const auto& email : emails) {
            if (strcmp(email.getUserName(), RxBuffer) == 0) {
                found = true;
                const EmailDetails& details = email.getEmailDetails();
                response += "From: " + std::string(details.from) + "\n";
                response += "To: " + std::string(details.to) + "\n";
                response += "Subject: " + std::string(details.subject) + "\n";
                response += "Body: " + std::string(details.body) + "\n**********\n";
            }
        }

        if (!found) {
            response = "No emails found for this user.";
        }

        send(clientSocket, response.c_str(), response.length(), 0);
    }
}


void SocketServer::Cleanup() {
    if (ServerSocket != INVALID_SOCKET) {
        closesocket(ServerSocket);
        ServerSocket = INVALID_SOCKET;
    }
    WSACleanup();
}
