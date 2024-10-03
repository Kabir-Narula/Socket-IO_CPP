#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include "Email.h"
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 0;

    SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ClientSocket == INVALID_SOCKET) {
        WSACleanup();
        return 0;
    }

    sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_port = htons(27000);
    SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR) {
        closesocket(ClientSocket);
        WSACleanup();
        return 0;
    }

    cout << "Connected to server.\nEnter username: ";
    string userName;
    cin >> userName;

    send(ClientSocket, userName.c_str(), userName.length(), 0);

    char RxBuffer[1024] = { 0 };
    int bytesReceived = recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);
    if (bytesReceived > 0) {
        cout << "Emails received:\n" << RxBuffer << endl;
    }
    else {
        cout << "No response from server." << endl;
    }

    cout << "\nPress ENTER to exit...";
    cin.ignore(); 
    cin.get(); 

    closesocket(ClientSocket);
    WSACleanup();
    return 0;
}
