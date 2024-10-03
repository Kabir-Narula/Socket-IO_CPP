#include <iostream>
#include <vector>
#include <winsock2.h>
#include "Email.h"
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 0;

    SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ServerSocket == INVALID_SOCKET) {
        WSACleanup();
        return 0;
    }

    sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_addr.s_addr = INADDR_ANY;
    SvrAddr.sin_port = htons(27000);
    if (bind(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR) {
        closesocket(ServerSocket);
        WSACleanup();
        return 0;
    }

    if (listen(ServerSocket, 1) == SOCKET_ERROR) {
        closesocket(ServerSocket);
        WSACleanup();
        return 0;
    }

    cout << "Waiting for client connections...\n";

    SOCKET ConnectionSocket;
    while (true) {
        ConnectionSocket = accept(ServerSocket, NULL, NULL);
        if (ConnectionSocket == SOCKET_ERROR) continue;

        cout << "Connection Established\n";

        char RxBuffer[128] = { 0 };
        int bytesReceived = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
        if (bytesReceived > 0) {
            RxBuffer[bytesReceived] = '\0'; // Ensure null-termination
            cout << "Client username received: " << RxBuffer << endl;
        }

        vector<Email> emails = {
            Email("JobsS", EmailDetails("Bill", "Jobs", "Finance Presentation", "Looking forward to our meeting on finance")),
            Email("BillG", EmailDetails("Warren", "Bill", "Dinner", "It was lovely meeting you over dinner")),
            Email("WarrenB", EmailDetails("Elon", "Warren", "SpaceX", "How's new SpaceX project going?")),
            Email("JobsS", EmailDetails("Elon", "Jobs", "Twitter", "How is it going at Twitter?"))
        };

        bool found = false;
        string allEmailsForUser;
        for (const auto& email : emails) {
            if (email.getUserName() == string(RxBuffer)) {
                found = true;
                allEmailsForUser += email.display() + "\n**********\n"; // Concatenate all emails for the user
            }
        }

        if (found) {
            cout << "Sent user inbox to client.\n";
            send(ConnectionSocket, allEmailsForUser.c_str(), allEmailsForUser.length(), 0);
        }
        else {
            cout << "No emails found for user. Sent message to client.\n";
            string msg = "No emails found for this user.";
            send(ConnectionSocket, msg.c_str(), msg.length(), 0);
        }

        closesocket(ConnectionSocket);
    }

    closesocket(ServerSocket);
    WSACleanup();
    return 0;
}
