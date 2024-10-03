#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include "socket.h"

// Function to calculate checksum using LFSR
uint16_t calculateChecksum(const char* data, int length) {
    uint16_t lfsr = 0xACE1u; // Initial seed
    for (int i = 0; i < length; i++) {
        lfsr ^= (unsigned char)data[i] << 8; // XOR with the character
        for (int j = 0; j < 8; j++) {
            if (lfsr & 0x8000) {
                lfsr = (lfsr << 1) ^ 0xB400; // Polynomial is 1011010000000000
            }
            else {
                lfsr <<= 1;
            }
        }
    }
    return lfsr & 0xFFFF;
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
        std::cerr << "Failed to accept client connection." << std::endl;
        return -1;
    }

    // Receive the username and checksum from the client
    std::string receivedMessage;
    int bytesRead = server.receive_message(receivedMessage);

    // Check that we have received enough data (checksum + username)
    if (bytesRead < sizeof(uint16_t) + 16) {
        std::cerr << "Received insufficient data." << std::endl;
        return -1;
    }

    // Extract received checksum and username
    uint16_t receivedChecksum;
    memcpy(&receivedChecksum, receivedMessage.data(), sizeof(uint16_t));

    char username[16];
    memcpy(username, receivedMessage.data() + sizeof(uint16_t), sizeof(username));

    // Calculate checksum for the received username
    uint16_t calculatedChecksum = calculateChecksum(username, std::strlen(username));

    // Check if the calculated checksum matches the received checksum
    if (calculatedChecksum != receivedChecksum) {
        std::cerr << "Checksum mismatch! Data may be corrupted." << std::endl;
        return -1;
    }

    // Email data
    std::map<std::string, std::vector<std::string>> emails = {
        {"JobsS", {"BillG: Finance Presentation - Looking forward to our meeting on finance."}},
        {"BillG", {"WarrenB: Dinner Plans - Shall we meet for dinner?"}},
        {"WarrenB", {"ElonM: SpaceX Update - New rocket design is ready for review."}},
        {"ElonM", {"JobsS: Twitter Project - Excited about our new collaboration."}}
    };

    // Find and concatenate emails associated with the username
    std::string allEmails;
    if (emails.find(username) != emails.end()) {
        for (const auto& email : emails[username]) {
            allEmails += email + "\n"; // Add newline separator
        }
    }

    // Calculate checksum for email data
    uint16_t emailChecksum = calculateChecksum(allEmails.c_str(), allEmails.size());

    // Prepare data to send back: checksum followed by email data
    std::vector<char> sendData(sizeof(emailChecksum) + allEmails.size());

    // Copy the checksum
    memcpy(sendData.data(), &emailChecksum, sizeof(emailChecksum));

    // Copy the email data
    memcpy(sendData.data() + sizeof(emailChecksum), allEmails.c_str(), allEmails.size());

    // Send the data back to the client
    server.send_message(std::string(sendData.data(), sendData.size()));

    return 0;
}
