#ifndef HTML_GENERATOR_H
#define HTML_GENERATOR_H

#include <vector>
#include <string>
#include "email.h"

std::string generateHTMLPage(const std::vector<Email>& emails);

#endif // HTML_GENERATOR_H
