#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <string.h> 
#include <chrono>   
#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Could not start DLLs" << std::endl;
        return 0;
    }

    SOCKET ClientSocket;
    ClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ClientSocket == INVALID_SOCKET) {
        std::cerr << "Could not create socket" << std::endl;
        WSACleanup();
        return 0;
    }

    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_port = htons(27020);
    SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    const int numMessages = 100000;
    char TxBuffer[128] = "Test message";
    char RxBuffer[128] = {};
    int addr_len = sizeof(SvrAddr);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numMessages; ++i) {
        sendto(ClientSocket, TxBuffer, sizeof(TxBuffer), 0, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr));
        recvfrom(ClientSocket, RxBuffer, sizeof(RxBuffer), 0, (struct sockaddr*)&SvrAddr, &addr_len);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cerr << "Total Time: " << elapsed.count() << " ms" << std::endl;

    closesocket(ClientSocket);
    WSACleanup();
    return 0;
}
