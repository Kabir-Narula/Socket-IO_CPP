#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "socket.h"

uint8_t calculate_checksum(const char* data, size_t length) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < length; i++) {
        checksum += data[i];
    }
    return checksum;
}

int main() {
    // Initialize and start the server
    Server_TCP server("127.0.0.1", 27000);
    if (!server.create_socket() || !server.bind_socket() || !server.listen_for_connections()) {
        std::cerr << "Server setup failed" << std::endl;
        return -1;
    }

    std::cout << "Waiting for a client to connect." << std::endl;
    if (!server.accept_connection()) {
        std::cerr << "Failed to accept client connection" << std::endl;
        return -1;
    }

    // Receive username and checksum
    std::string username;
    server.receive_message(username);

    std::string received_checksum_str;
    server.receive_message(received_checksum_str);

    uint8_t received_checksum = static_cast<uint8_t>(std::stoi(received_checksum_str));

    // Calculate checksum for the received username
    uint8_t calculated_checksum = calculate_checksum(username.c_str(), username.size());

    // Verify checksum
    if (calculated_checksum != received_checksum) {
        std::cerr << "Checksum mismatch for username. Possible data corruption." << std::endl;
        return -1;
    }

    // Emails data
    std::map<std::string, std::vector<std::string>> emails = {
        {"JobsS", {"BillG: Finance Presentation - Looking forward to our meeting on finance."}},
        {"BillG", {"WarrenB: Dinner Plans - Shall we meet for dinner?"}},
        {"WarrenB", {"ElonM: SpaceX Update - New rocket design is ready for review."}},
        {"ElonM", {"JobsS: Twitter Project - Excited about our new collaboration."}}
    };

    // Prepare emails and calculate checksum
    std::string allEmails;
    if (emails.find(username) != emails.end()) {
        for (const auto& email : emails[username]) {
            allEmails += email + "\n"; // New line as separator
        }
    }

    if (!allEmails.empty()) {
        // Calculate checksum for the emails
        uint8_t email_checksum = calculate_checksum(allEmails.c_str(), allEmails.size());

        // Convert email checksum to a string
        std::ostringstream oss;
        oss << static_cast<int>(email_checksum);
        std::string email_checksum_str = oss.str();

        // Send emails and checksum as separate messages
        server.send_message(allEmails);
        server.send_message(email_checksum_str);
    }
    else {
        // Handle case where no emails were found
        std::cout << "No emails found for username: " << username << std::endl;
    }

    return 0;
}
