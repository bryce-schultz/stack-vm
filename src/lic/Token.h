#pragma once

#include <string>

#include "Location.h"

enum TokenType
{
	NONE = 1000,
	NUMBER,
	PRINT,
	IDENTIFIER,
	STRING,
	ASM,
	END,
    FOR,
	JUNK
};

std::string tokenTypeToString(int type);

class Token
{
public:
    Token(int type, const std::string &text, const Location &location);

    bool operator==(const Token &other) const;
    bool operator==(int type) const;
    bool operator==(char type) const;

    bool operator!=(const Token &other) const;
    bool operator!=(int type) const;
    bool operator!=(char type) const;

    operator char() const;

    int getType() const;
    size_t getLength() const;
    const std::string &getText() const;
    const Location &getLocation() const;
private:
    int type;
    std::string text;

    Location location;
};

#define TOKEN_NONE { NONE, "", {0, 0, ""} }