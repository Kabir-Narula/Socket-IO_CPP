#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <winsock2.h>
#include "Email.h" 
class SocketServer {
public:
    SocketServer();
    ~SocketServer();
    bool InitializeWinSock();
    bool CreateSocket();
    bool BindSocket(int port);
    bool ListenForClients();
    bool AcceptClient();
    void Cleanup();
private:
    SOCKET ServerSocket;
    void HandleClient(SOCKET clientSocket);
};
