#include <iostream>
#include <map>
#include <vector>
#include "socket.h"
#include "email.h"
#include "html_generator.h"

int main() {
    // Initialize and start the server
    Server_TCP server("127.0.0.1", 27000);
    if (!server.create_socket() || !server.bind_socket() || !server.listen_for_connections()) {
        std::cerr << "Server setup failed" << std::endl;
        return -1;
    }

    std::cout << "Waiting for a client to connect." << std::endl;

    while (true) { // Loop to continuously accept connections
        if (!server.accept_connection()) {
            std::cerr << "Failed to accept client connection" << std::endl;
            continue; // Continue to next iteration to keep accepting connections
        }

        // Receive the URL with the username from the client
        std::string url;
        server.receive_message(url);

        // Extract the username from the URL
        std::string username;
        size_t startPos = url.find("username=") + 9; // Length of "username=" is 9
        size_t endPos = url.find(" ", startPos);
        if (startPos != std::string::npos && endPos != std::string::npos) {
            username = url.substr(startPos, endPos - startPos);
        }
        else {
            std::cerr << "Invalid URL format" << std::endl;
            // Send error response to client
            server.send_message("HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nInvalid URL format");
            continue; // Continue to next iteration to keep accepting connections
        }

        // Print the connected username to the server terminal
        std::cout << "Connected to client with username: " << username << std::endl;

        // Emails data
        std::vector<Email> emails = {
            Email("JobsS", EmailDetails("Bill", "Jobs", "Finance Presentation", "Looking forward to our meeting on finance")),
            Email("BillG", EmailDetails("Warren", "Bill", "Dinner", "It was lovely meeting you over dinner")),
            Email("WarrenB", EmailDetails("Elon", "Warren", "SpaceX", "How's new SpaceX project going?")),
            Email("JobsS", EmailDetails("Elon", "Jobs", "Twitter", "How is it going at Twitter?"))
        };

        // Filter emails based on the provided username
        std::vector<Email> userEmails;
        bool userFound = false;
        for (const auto& email : emails) {
            if (email.getUsername() == username) {
                userEmails.push_back(email);
                userFound = true;
            }
        }

        // If user not found, send error response
        if (!userFound) {
            std::cerr << "User not found: " << username << std::endl;
            server.send_message("HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nUser not found: " + username);
            continue; // Continue to next iteration to keep accepting connections
        }

        // Prepare inbox data as HTML page
        std::string htmlResponse = generateHTMLPage(userEmails);

        // Add HTTP headers
        std::string httpResponse = "HTTP/1.1 200 OK\r\n";
        httpResponse += "Content-Type: text/html\r\n";
        httpResponse += "Content-Length: " + std::to_string(htmlResponse.size()) + "\r\n";
        httpResponse += "\r\n"; // Empty line indicates end of headers
        httpResponse += htmlResponse;

        // Send HTML response to client
        server.send_message(httpResponse);
    }

    return 0;
}
