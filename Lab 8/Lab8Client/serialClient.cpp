#include <iostream>
#include <string>
#include <cstring>
#include "socket.h"

// Function to calculate checksum using LFSR
uint16_t calculateChecksum(const char* data, int length) {
    uint16_t lfsr = 0xACE1u;
    for (int i = 0; i < length; i++) {
        lfsr ^= (unsigned char)data[i] << 8;
        for (int j = 0; j < 8; j++) {
            if (lfsr & 0x8000) {
                lfsr = (lfsr << 1) ^ 0xB400;
            }
            else {
                lfsr <<= 1;
            }
        }
    }
    return lfsr & 0xFFFF;
}

int main() {
    char username[16];
    std::cout << "Enter username: ";
    std::cin.getline(username, sizeof(username));

    // Calculate checksum for the username
    uint16_t usernameChecksum = calculateChecksum(username, std::strlen(username));

    // Prepare the data to send (checksum + username)
    char dataToSend[18];
    memcpy(dataToSend, &usernameChecksum, sizeof(uint16_t)); // Copy checksum
    memcpy(dataToSend + sizeof(uint16_t), username, sizeof(username)); // Copy username

    // Create a TCP client and connect to the server
    Client_TCP client;
    client.create_socket();
    if (!client.connect_socket("127.0.0.1", 27000)) {
        std::cerr << "Failed to connect to server." << std::endl;
        return -1;
    }

    // Send the data to the server
    if (client.send_message(std::string(dataToSend, sizeof(dataToSend))) < 0) {
        std::cerr << "Failed to send data to server." << std::endl;
        return -1;
    }

    // Receive the response from the server
    std::string response;
    int numBytesReceived = client.receive_message(response);

    // Check that we have received enough data (checksum + email data)
    if (numBytesReceived < sizeof(uint16_t)) {
        std::cerr << "Received insufficient data." << std::endl;
        return -1;
    }

    // Extract received checksum and data
    uint16_t receivedChecksum;
    memcpy(&receivedChecksum, response.data(), sizeof(uint16_t)); // Deserialize checksum
    std::string receivedData = response.substr(2); // Remaining data after checksum

    // Calculate checksum for received data
    uint16_t calculatedChecksum = calculateChecksum(receivedData.c_str(), receivedData.size());

    // Check if checksums match
    if (calculatedChecksum != receivedChecksum) {
        std::cerr << "Checksum mismatch! Data may be corrupted." << std::endl;
        return -1;
    }

    // Display the received email data
    std::cout << "Data received from server: " << receivedData << std::endl;

    return 0;
}
