#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "socket.h"

// Structure to represent email details
struct EmailDetails {
    std::string from;
    std::string to;
    std::string subject;
    std::string body;

    EmailDetails(const std::string& from, const std::string& to, const std::string& subject, const std::string& body)
        : from(from), to(to), subject(subject), body(body) {}
};

// Structure to represent an email
struct Email {
    std::string username;
    EmailDetails details;

    Email(const std::string& username, const EmailDetails& details)
        : username(username), details(details) {}
};

// Function to generate HTML page for inbox data
std::string generateHTMLPage(const std::vector<Email>& emails, const std::string& username) {
    std::string htmlPage = "<!DOCTYPE html>\n";
    htmlPage += "<html>\n<head>\n<title>Inbox</title>\n";
    htmlPage += "<style>\n";
    htmlPage += "body {\n";
    htmlPage += "    font-family: Arial, sans-serif;\n";
    htmlPage += "    background-color: #f4f4f4;\n";
    htmlPage += "    margin: 0;\n";
    htmlPage += "    padding: 0;\n";
    htmlPage += "}\n";
    htmlPage += "h1 {\n";
    htmlPage += "    color: #333;\n";
    htmlPage += "    text-align: center;\n";
    htmlPage += "}\n";
    htmlPage += "table {\n";
    htmlPage += "    border-collapse: collapse;\n";
    htmlPage += "    width: 80%;\n";
    htmlPage += "    margin: 20px auto;\n";
    htmlPage += "    background-color: #fff;\n";
    htmlPage += "    border-radius: 10px;\n";
    htmlPage += "    box-shadow: 0 0 20px rgba(0, 0, 0, 0.1);\n";
    htmlPage += "}\n";
    htmlPage += "th, td {\n";
    htmlPage += "    border: 1px solid #ddd;\n";
    htmlPage += "    padding: 12px;\n";
    htmlPage += "    text-align: left;\n";
    htmlPage += "}\n";
    htmlPage += "th {\n";
    htmlPage += "    background-color: #f2f2f2;\n";
    htmlPage += "}\n";
    htmlPage += "tr:nth-child(even) {\n";
    htmlPage += "    background-color: #f9f9f9;\n";
    htmlPage += "}\n";
    htmlPage += "tr:hover {\n";
    htmlPage += "    background-color: #f1f1f1;\n";
    htmlPage += "}\n";
    htmlPage += "</style>\n";
    htmlPage += "</head>\n<body>\n";
    htmlPage += "<h1>Inbox</h1>\n";
    htmlPage += "<table>\n";
    htmlPage += "<tr><th>From</th><th>To</th><th>Subject</th><th>Body</th></tr>\n";
    for (const Email& email : emails) {
        if (email.username == username) {
            htmlPage += "<tr>";
            htmlPage += "<td>" + email.details.from + "</td>";
            htmlPage += "<td>" + email.details.to + "</td>";
            htmlPage += "<td>" + email.details.subject + "</td>";
            htmlPage += "<td>" + email.details.body + "</td>";
            htmlPage += "</tr>\n";
        }
    }
    htmlPage += "</table>\n";
    htmlPage += "</body>\n</html>\n";
    return htmlPage;
}

int main() {
    // Initialize and start the server
    Server_TCP server("127.0.0.1", 27000);
    if (!server.create_socket() || !server.bind_socket() || !server.listen_for_connections()) {
        std::cerr << "Server setup failed" << std::endl;
        return -1;
    }

    while (true) {
        std::cout << "Waiting for a client to connect." << std::endl;
        if (!server.accept_connection()) {
            std::cerr << "Failed to accept client connection" << std::endl;
            continue;
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

        // Emails data
        std::vector<Email> emails = {
            Email("JobsS", EmailDetails("Bill", "Jobs", "Finance Presentation", "Looking forward to our meeting on finance")),
            Email("BillG", EmailDetails("Warren", "Bill", "Dinner", "It was lovely meeting you over dinner")),
            Email("WarrenB", EmailDetails("Elon", "Warren", "SpaceX", "How's new SpaceX project going?")),
            Email("JobsS", EmailDetails("Elon", "Jobs", "Twitter", "How is it going at Twitter?"))
        };

        // Prepare inbox data as HTML page
        std::string htmlResponse = generateHTMLPage(emails, username);

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
