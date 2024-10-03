#include "email.h"

Email::Email(const std::string& username, const EmailDetails& details)
    : username(username), details(details) {}

std::string Email::getUsername() const {
    return username;
}

EmailDetails Email::getDetails() const {
    return details;
}
