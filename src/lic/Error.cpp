//***********************************************
// Error.cpp
//***********************************************

#include <sstream>
#include <fstream>
#include <iostream>

#include "Error.h"
#include "Color.h"

namespace global
{
    bool hadError = false;
}

std::string getSquiggles(const Token &token)
{
    const int length = token.getType() == STRING ? token.getText().length() + 2 : token.getText().length();
    const int start = token.getLocation().getColumn() - 1;
    std::string squiggles = "";

    for (int i = 0; i < start; i++)
    {
        squiggles += " ";
    }

    for (int i = 0; i < length; i++)
    {
        squiggles += "~";
    }

    return squiggles;
}

std::string getLine(const Token &token)
{
    std::ifstream file(token.getLocation().getFilename());
    const int desiredLine = token.getLocation().getLine();

    if (!file.is_open())
    {
        return "";
    }

    std::string line;
    int i = 1;

    while (std::getline(file, line))
    {
        if (i == desiredLine)
        {
            return line;
        }

        i++;
    }

    return "";
}

std::string getSquiggleLine(const Token &token)
{
    std::string line = getLine(token);
    std::string squiggles = getSquiggles(token);
    return "│  " + line + "\n│  " + red + squiggles + reset;
}

std::string getErrorHeader(const Token &token, const std::string &message)
{
    std::stringstream ss;
    const std::string &filename = token.getLocation().getFilename();
    const int linenum = token.getLocation().getLine();
    const int colnum = token.getLocation().getColumn();

    ss << red << "error" << reset << ": " << filename << ":" << linenum << ":" << colnum << ": " << message << "\n";
    return ss.str();
}

std::string getError(const Token &token, const std::string &message)
{
    return getErrorHeader(token, message) + getSquiggleLine(token) + "\n";
}

void error_(const std::string &message, const Token &token, const std::string &file, int line)
{
    const bool showErrorSourceLocation = true;

    std::stringstream ss;
    ss << getError(token, message);
    if (showErrorSourceLocation)
    {
        ss << "╰─ @ " << file << ":" << line << "\n";
    }

    // Print the error message to stderr.
    std::cerr << ss.str();

    // Set the global error flag.
    global::hadError = true;
}