#include "Email.h"

Email::Email() : userName("Default"), details("Zara", "Class", "Announcement", "Welcome to BTN") {}

Email::Email(std::string uName, EmailDetails det) : userName(uName), details(det) {}

std::string Email::display() const {
    return "From: " + details.from + "\nTo: " + details.to + "\nSubject: " + details.subject + "\nBody: " + details.body;
}
