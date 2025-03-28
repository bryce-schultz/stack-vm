//***********************************************
// LithiumTokenizer.h
//***********************************************

#pragma once

#include <string>

#include "Token.h"

class LithiumTokenizer
{
public:
    LithiumTokenizer();
    LithiumTokenizer(const std::string &source, const std::string &filename = "");
    Token peekToken();
	Token nextToken();
    bool eof();
private:
	bool isWhitespace(char c) const;
    void skipWhitespace();
    void skipComments();
    Token getToken();

    char peek() const;
    char next();

    Location getLocation() const;
private:
    std::string source;
    std::string filename;
    std::string text;
    Token currentToken;
    size_t index;
};