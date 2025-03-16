#pragma once

#include <string>

#include "Token.h"

namespace global
{
    extern bool hadError;
}

#define error(message, toekn) error_(message, toekn, __FILE__, __LINE__)
#define expected(message, token) error(std::string("expected ") + message + " but got " + token.getText() + " instead", token)

void error_(const std::string &message, const Token &token, const std::string &file, int line);