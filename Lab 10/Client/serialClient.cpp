#include <iostream>
#include <string>
#include "socket.h"

int main() {
    // Get the username from user input
    std::string username;
    std::cout << "Enter username: ";
    std::cin >> username;

    // Construct the URL with the username
    std::string url = "GET /?username=" + username + " HTTP/1.1\r\n\r\n";

    // Create and connect the client socket
    Client_TCP client;
    client.create_socket();
    client.connect_socket("127.0.0.1", 27000);

    // Send the URL with the username to the server
    client.send_message(url);

    // Receive and display the HTML response from the server
    std::string htmlResponse;
    client.receive_message(htmlResponse);
    std::cout << "Received HTML response:\n" << htmlResponse << std::endl;

    return 0;
}
