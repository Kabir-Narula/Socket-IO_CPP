#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    // Starts Winsock DLLs
    WSADATA wsaData;
    if ((WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
        return 0;
    }

    // Initializes socket. SOCK_STREAM: TCP
    SOCKET ClientSocket;
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ClientSocket == INVALID_SOCKET) {
        WSACleanup();
        return 0;
    }

    // Connects socket to specified server
    sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_port = htons(27000);
    if (inet_pton(AF_INET, "127.0.0.1", &SvrAddr.sin_addr) != 1) {
        closesocket(ClientSocket);
        WSACleanup();
        return 0;
    }

    if ((connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
        closesocket(ClientSocket);
        WSACleanup();
        return 0;
    }

    while (1) {
        float weight;
        std::cout << "Enter a weight in lbs (negative to exit): ";
        std::cin >> weight;

        if (weight < 0) {
            break;
        }
        // Send the weight to the server
        send(ClientSocket, reinterpret_cast<char*>(&weight), sizeof(weight), 0);
    }

    // Closes connection and socket
    closesocket(ClientSocket);
    // Frees Winsock DLL resources
    WSACleanup();

    return 0;
}
