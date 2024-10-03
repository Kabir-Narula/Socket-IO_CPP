
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <string.h> 
#include <chrono>   
#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return 0; 
    }

    SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ClientSocket == INVALID_SOCKET) {
        WSACleanup();
        return 0; 
    }

    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_port = htons(27000);
    SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR) {
        closesocket(ClientSocket);
        WSACleanup();
        return 0; 
    }

    char TxBuffer[128] = "Test message";
    char RxBuffer[128] = {};
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 100000; ++i) {
        send(ClientSocket, TxBuffer, sizeof(TxBuffer), 0);
        recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cout << "Total Time: " << elapsed.count() << " ms" << std::endl;

    closesocket(ClientSocket);
    WSACleanup();
    return 0;
}
