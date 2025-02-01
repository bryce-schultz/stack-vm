#include "TokenError.h"

TokenError::TokenError(const std::string &message, const Token &token):
    Error(message, token.getLocation())
{
}