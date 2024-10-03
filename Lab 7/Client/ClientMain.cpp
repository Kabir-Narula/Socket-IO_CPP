#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "SocketClient.h"
#include <iostream>

int main() {
    SocketClient client;
    if (!client.InitializeWinSock()) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }
    if (!client.CreateSocket()) {
        std::cerr << "Socket creation failed." << std::endl;
        return 1;
    }
    if (!client.ConnectToServer("127.0.0.1", 27000)) {
        std::cerr << "Failed to connect to server." << std::endl;
        return 1;
    }

    std::cout << "Connected to server.\nEnter username: ";
    std::string userName;
    std::cin >> userName;

    std::string response = client.SendAndReceive(userName.c_str());
    std::cout << "Emails received:\n" << response << std::endl;

    client.Cleanup();
    return 0;
}
