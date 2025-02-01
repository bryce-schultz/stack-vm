#pragma once

#include "Error.h"
#include "Token.h"

class TokenError : public Error
{
public:
    TokenError(const std::string &message, const Token &token);
};