#include "SocketServer.h"
#include <iostream>

int main() {
    SocketServer server;
    if (!server.InitializeWinSock()) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }
    if (!server.CreateSocket()) {
        std::cerr << "Socket creation failed." << std::endl;
        return 1;
    }
    if (!server.BindSocket(27000)) {
        std::cerr << "Socket bind failed." << std::endl;
        return 1;
    }
    if (!server.ListenForClients()) {
        std::cerr << "Listening for clients failed." << std::endl;
        return 1;
    }

    std::cout << "Waiting for client connections...\n";
    while (server.AcceptClient()) {
        // Loop to accept new connections
    }

    server.Cleanup();
    return 0;
}
