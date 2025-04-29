//***********************************************
// Error.h
//***********************************************

#pragma once

#include <string>

#include "Token.h"

namespace global
{
    // Global variable to keep track of whether an error has occurred throughout the various compiler steps.
    extern bool hadError;
}

// Error macros for consistant error message formatting.
#define error(message, token) token_error(message, token, __FILE__, __LINE__)
#define expected(message, token) error(std::string("expected ") + message + std::string(" but got '") + token.getText() + (token == JUNK ? "' (JUNK) instead" : "' instead"), token)
#define undefined(token) error("variable " + token.getText() + " is not defined", token);
#define undefined_function(token) error("function " + token.getText() + " is not defined", token);

#define general_error(message) error_(message, __FILE__, __LINE__)

//***********************************************
// token_error
//
// Internal error function that takes the message,
// token, file, and line number. Where the file 
// and line number are automatically filled in by
//  the error macro.
//***********************************************
void token_error(const std::string &message, const Token &token, const std::string &file, int line);

//***********************************************
// error_
// 
// Internal error function that takes the message,
// file, and line number. Where the file 
// and line number are automatically filled in by
// the general_error macro.
//***********************************************
void error_(const std::string &message, const std::string &file, int line);