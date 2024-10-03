#include <iostream>
#include <string>
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
    char username[16]; // Assuming usernames are no longer than 15 characters + null terminator
    std::cout << "Enter username: ";
    std::cin.getline(username, sizeof(username));

    Client_TCP client;
    client.create_socket();
    client.connect_socket("127.0.0.1", 27000);

    // Calculate and send checksum for the username
    uint8_t username_checksum = calculate_checksum(username, strlen(username));

    // Convert checksum to string
    std::ostringstream oss;
    oss << static_cast<int>(username_checksum);
    std::string checksum_str = oss.str();

    // Send username and checksum as separate messages
    client.send_message(std::string(username));
    client.send_message(checksum_str);

    // Receive emails and checksum
    std::string emails;
    client.receive_message(emails);

    std::string received_checksum_str;
    client.receive_message(received_checksum_str);

    uint8_t received_checksum = static_cast<uint8_t>(std::stoi(received_checksum_str));

    // Calculate checksum for received emails
    uint8_t calculated_checksum = calculate_checksum(emails.c_str(), emails.size());

    // Verify checksums
    if (calculated_checksum == received_checksum) {
        // Display emails
        std::cout << "Emails associated with " << username << ":\n" << emails << std::endl;
    }
    else {
        std::cerr << "Checksum mismatch. Received data might be corrupted." << std::endl;
    }

    return 0;
}
