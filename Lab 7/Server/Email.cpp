#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Email.h"
#include <cstring>

char* allocateAndCopy(const char* source) {
    if (source == nullptr) {
        return nullptr;
    }
    char* dest = new char[strlen(source) + 1];
    strcpy_s(dest, strlen(source) + 1, source);
    return dest;
}

// EmailDetails implementations
EmailDetails::EmailDetails() : from(nullptr), to(nullptr), subject(nullptr), body(nullptr) {}

EmailDetails::EmailDetails(const char* f, const char* t, const char* s, const char* b)
    : from(allocateAndCopy(f)), to(allocateAndCopy(t)),
    subject(allocateAndCopy(s)), body(allocateAndCopy(b)) {}

EmailDetails::~EmailDetails() {
    delete[] from;
    delete[] to;
    delete[] subject;
    delete[] body;
}

EmailDetails::EmailDetails(const EmailDetails& other)
    : from(allocateAndCopy(other.from)), to(allocateAndCopy(other.to)),
    subject(allocateAndCopy(other.subject)), body(allocateAndCopy(other.body)) {}

EmailDetails& EmailDetails::operator=(const EmailDetails& other) {
    if (this != &other) {
        delete[] from;
        delete[] to;
        delete[] subject;
        delete[] body;

        from = allocateAndCopy(other.from);
        to = allocateAndCopy(other.to);
        subject = allocateAndCopy(other.subject);
        body = allocateAndCopy(other.body);
    }
    return *this;
}

// Email implementations
Email::Email() : userName(nullptr), details() {}

Email::Email(const char* uName, const EmailDetails& det)
    : userName(allocateAndCopy(uName)), details(det) {}

Email::~Email() {
    delete[] userName;
}

Email::Email(const Email& other)
    : userName(allocateAndCopy(other.userName)), details(other.details) {}

Email& Email::operator=(const Email& other) {
    if (this != &other) {
        delete[] userName;
        userName = allocateAndCopy(other.userName);
        details = other.details; 
    }
    return *this;
}

const char* Email::getUserName() const {
    return userName;
}

const EmailDetails& Email::getEmailDetails() const {
    return details;
}
