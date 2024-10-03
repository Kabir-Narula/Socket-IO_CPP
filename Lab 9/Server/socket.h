
#include <string>
#include <iostream>
#include<winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#define MAX_BUFFER_SIZE 512

struct packet {
    char name[16];
    bool flag;
    int size;
    char* letters;
};

struct serialized_packet {
    char* data;
    int length;
};

packet create_packet();
struct serialized_packet packet_serializer(packet);
packet packet_deserializer(char*);

class Node {
public:
    static bool dlls_started;
    static int num_nodes;
    std::string ip;
    std::string role;
    std::string protocol;
    int port;
    SOCKET active_socket;
public:
    Node();
    Node(std::string ip, int port);
    ~Node();
    void start_dlls() const;
    bool create_socket();
    void display_info() const;
};

class Server_TCP : public Node {
private:
    SOCKET client_socket;
public:
    Server_TCP();
    Server_TCP(std::string ip, int port);
    ~Server_TCP();
    bool bind_socket();
    bool listen_for_connections();
    bool accept_connection();
    int send_message(std::string message);
    int send_packet(packet message);
    int receive_message(std::string& message);
    int receive_packet(packet& my_packet);
    void close_connection();
};

class Client_TCP : public Node {
public:
    Client_TCP();
    bool connect_socket(std::string ip, int port);
    int send_message(std::string message);
    int send_packet(packet message);
    int receive_message(std::string& message);
    int receive_packet(packet& my_packet);
};

class Client_UDP : public Node {
public:
    Client_UDP();
    int send_message(std::string message, std::string ip, int port);
    struct sockaddr_in receive_message(std::string& message);
};

class Server_UDP : public Node {
public:
    Server_UDP();
    Server_UDP(std::string ip, int port);
    bool bind_socket();
    int send_message(std::string message, std::string ip, int port);
    struct sockaddr_in receive_message(std::string& message);
};
