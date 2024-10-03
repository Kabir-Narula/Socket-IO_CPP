#include "html_generator.h"

std::string generateHTMLPage(const std::vector<Email>& emails) {
    std::string htmlPage = "<!DOCTYPE html>\n";
    htmlPage += "<html>\n<head>\n<title>Inbox</title>\n";
    htmlPage += "<style>\n";
    htmlPage += "table {\nborder-collapse: collapse;\nwidth: 100%;\n}\n";
    htmlPage += "th, td {\nborder: 1px solid #dddddd;\ntext-align: left;\n";
    htmlPage += "padding: 8px;\n}\n";
    htmlPage += "th {\nbackground-color: #f2f2f2;\n}\n";
    htmlPage += "</style>\n";
    htmlPage += "</head>\n<body>\n";
    htmlPage += "<h1>Inbox</h1>\n";
    htmlPage += "<table>\n";
    htmlPage += "<tr><th>From</th><th>To</th><th>Subject</th><th>Body</th></tr>\n";
    for (const Email& email : emails) {
        EmailDetails details = email.getDetails();
        htmlPage += "<tr>";
        htmlPage += "<td>" + details.from + "</td>";
        htmlPage += "<td>" + details.to + "</td>";
        htmlPage += "<td>" + details.subject + "</td>";
        htmlPage += "<td>" + details.body + "</td>";
        htmlPage += "</tr>\n";
    }
    htmlPage += "</table>\n";
    htmlPage += "</body>\n</html>\n";
    return htmlPage;
}
