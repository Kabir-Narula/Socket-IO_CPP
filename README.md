
# Socket Programming in C++ Cheatsheet

## Overview
This README provides a comprehensive cheatsheet for socket programming in C++, covering key definitions, socket types, basic server-client code structure, system calls, and common debugging techniques. It is designed for students and professionals seeking a quick reference for socket programming concepts and coding patterns.

---

## 1. Socket Types

### 1.1 Stream Socket
- **Protocol:** TCP  
- **Use Case:** Reliable, connection-oriented communication.

### 1.2 Datagram Socket
- **Protocol:** UDP  
- **Use Case:** Connectionless, fast, but not reliable.

### 1.3 Raw Socket
- **Protocol:** IP Protocol  
- **Use Case:** Direct access to lower network layers.

### 1.4 Server Socket
- **Protocol:** TCP/UDP  
- **Use Case:** Listens for incoming client connections.

### 1.5 Client Socket
- **Protocol:** TCP/UDP  
- **Use Case:** Connects to server to send/receive data.

---

## 2. Socket Programming Essentials

### 2.1 Header Files
Include the necessary header files for socket programming:

\`\`\`cpp
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
\`\`\`

### 2.2 Socket Creation
Create a socket using the `socket()` function:

\`\`\`cpp
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
\`\`\`

### 2.3 Bind Address to Socket
Bind an address to a socket with `bind()`:

\`\`\`cpp
struct sockaddr_in server_addr;
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(8080);

bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
\`\`\`

### 2.4 Listen and Accept Connections
Set the socket to listen for connections and accept incoming connections:

\`\`\`cpp
listen(sockfd, 5);
int client_socket = accept(sockfd, (struct sockaddr*)NULL, NULL);
\`\`\`

### 2.5 Connect to Server (Client Side)
To connect to a server using a client socket:

\`\`\`cpp
struct sockaddr_in server_addr;
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(8080);
inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
\`\`\`

---

## 3. System Calls Overview

### 3.1 Common System Calls
- **socket()**: Creates a socket and returns a file descriptor.
- **bind()**: Binds a socket to a specific IP address and port.
- **listen()**: Prepares a socket to accept incoming connections.
- **accept()**: Blocks the program until a connection is received.
- **connect()**: Connects a client to a remote server.
- **send()**: Sends data to a connected socket.
- **recv()**: Receives data from a connected socket.
- **close()**: Closes a socket.

### 3.2 Error Handling
Use `perror()` to print error messages:

\`\`\`cpp
if (sockfd < 0) {
    perror("Socket creation failed");
}
\`\`\`

---

## 4. Basic Server Example

\`\`\`cpp
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);
    
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    char *message = "Hello from server";
    send(new_socket, message, strlen(message), 0);
    close(server_fd);
    return 0;
}
\`\`\`

---

## 5. Basic Client Example

\`\`\`cpp
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    send(sock, "Hello from client", strlen("Hello from client"), 0);
    read(sock, buffer, 1024);
    std::cout << buffer << std::endl;
    close(sock);
    return 0;
}
\`\`\`

---

## 6. Debugging Techniques

### 6.1 Common Errors
- **Address Already in Use**: Occurs when binding to an already-used port.
- **Connection Refused**: The server may not be listening on the specified port.
- **Broken Pipe**: Happens when the server terminates unexpectedly.

### 6.2 Debugging Tips
- Use `netstat -tulnp` to check active ports.
- Use `strace` or `gdb` for in-depth debugging.
