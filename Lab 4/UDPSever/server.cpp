#include <iostream>
#include <winsock2.h>
#include <string.h> 
#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return 0;
    }

    SOCKET ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ServerSocket == INVALID_SOCKET) {
        WSACleanup();
        return 0; 
    }

    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_addr.s_addr = INADDR_ANY;
    SvrAddr.sin_port = htons(27020);
    if (bind(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR) {
        closesocket(ServerSocket);
        WSACleanup();
        return 0; 
    }

    char RxBuffer[128] = {};
    char TxBuffer[128] = "Ack";
    struct sockaddr_in CltAddr;
    int addr_len = sizeof(CltAddr);

    for (int i = 0; i < 100000; ++i) {
        recvfrom(ServerSocket, RxBuffer, sizeof(RxBuffer), 0, (struct sockaddr*)&CltAddr, &addr_len);
        sendto(ServerSocket, TxBuffer, sizeof(TxBuffer), 0, (struct sockaddr*)&CltAddr, sizeof(CltAddr));
    }

    closesocket(ServerSocket);
    WSACleanup();
    return 0;
}
