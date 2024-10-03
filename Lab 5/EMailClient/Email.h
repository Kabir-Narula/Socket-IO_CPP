#pragma once

#include <string>

struct EmailDetails {
    std::string from;
    std::string to;
    std::string subject;
    std::string body;


    EmailDetails() : from(""), to(""), subject(""), body("") {}


    EmailDetails(std::string f, std::string t, std::string s, std::string b)
        : from(f), to(t), subject(s), body(b) {}
};

class Email {
public:
   
    Email();
    Email(std::string userName, EmailDetails details);


    std::string getUserName() const { return userName; }

    
    std::string display() const;

private:
    std::string userName;
    EmailDetails details;
};
