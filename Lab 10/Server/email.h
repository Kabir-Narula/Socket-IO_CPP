#ifndef EMAIL_H
#define EMAIL_H

#include <string>

struct EmailDetails {
    std::string from;
    std::string to;
    std::string subject;
    std::string body;

    EmailDetails(const std::string& from, const std::string& to, const std::string& subject, const std::string& body)
        : from(from), to(to), subject(subject), body(body) {}
};

class Email {
private:
    std::string username;
    EmailDetails details;

public:
    Email(const std::string& username, const EmailDetails& details);

    std::string getUsername() const;
    EmailDetails getDetails() const;
};

#endif // EMAIL_H
