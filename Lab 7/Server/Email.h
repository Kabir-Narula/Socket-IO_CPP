#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#pragma once


struct EmailDetails {
    char* from;
    char* to;
    char* subject;
    char* body;

    EmailDetails();
    EmailDetails(const char* f, const char* t, const char* s, const char* b);
    ~EmailDetails();
    EmailDetails(const EmailDetails& other);
    EmailDetails& operator=(const EmailDetails& other);
};

class Email {
public:
    Email();
    Email(const char* userName, const EmailDetails& details);
    ~Email();
    Email(const Email& other);
    Email& operator=(const Email& other);

    const char* getUserName() const;
    const EmailDetails& getEmailDetails() const;

private:
    char* userName;
    EmailDetails details;
};
