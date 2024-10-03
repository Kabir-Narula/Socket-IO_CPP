#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

class SocketClient {
public:
    SocketClient();
    ~SocketClient();
    bool InitializeWinSock();
    bool CreateSocket();
    bool ConnectToServer(const char* ip, int port);
    char* SendAndReceive(const char* message); 
    void Cleanup();
private:
    SOCKET ClientSocket;
};
