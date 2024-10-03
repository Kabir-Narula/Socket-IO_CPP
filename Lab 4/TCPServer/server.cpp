#include <iostream>
#include <winsock2.h>
#include <string.h>
#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return 0; 
    }

    SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        WSACleanup();
        return 0; 
    }

    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_addr.s_addr = INADDR_ANY;
    SvrAddr.sin_port = htons(27000);
    if (bind(ListenSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR) {
        closesocket(ListenSocket);
        WSACleanup();
        return 0; 
    }

    if (listen(ListenSocket, 1) == SOCKET_ERROR) {
        closesocket(ListenSocket);
        WSACleanup();
        return 0; 
    }

    SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        closesocket(ListenSocket);
        WSACleanup();
        return 0; 
    }

    closesocket(ListenSocket); 

    char RxBuffer[128] = {};
    char TxBuffer[128] = "Ack";
    for (int i = 0; i < 100000; ++i) {
        recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);
        send(ClientSocket, TxBuffer, sizeof(TxBuffer), 0);
    }

    closesocket(ClientSocket);
    WSACleanup();
    return 0;
}
