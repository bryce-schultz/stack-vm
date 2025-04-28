//***********************************************
// LithiumTokenizer.cpp
//***********************************************

#include "LithiumTokenizer.h"
#include "Error.h"

LithiumTokenizer::LithiumTokenizer():
    source(""),
    filename(""),
    currentToken(TOKEN_NONE),
    index(0)
{
}

LithiumTokenizer::LithiumTokenizer(const std::string &source, const std::string &filename):
    source(source),
    filename(filename),
    currentToken(TOKEN_NONE),
    index(0)
{
}

Token LithiumTokenizer::peekToken()
{
    if (currentToken == NONE)
    {
        currentToken = getToken();
    }

    return currentToken;
}

Token LithiumTokenizer::nextToken()
{
    currentToken = getToken();
    return currentToken;
}

bool LithiumTokenizer::eof()
{
    skipWhitespace();
    return index >= source.size();
}

bool LithiumTokenizer::isWhitespace(char c) const
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void LithiumTokenizer::skipWhitespace()
{
    char c = peek();

    while(isWhitespace(c))
    {
        c = next();
    }
}

void LithiumTokenizer::skipComments()
{
    char c = peek();

    if (c == '#')
    {
        while (c != '\n' && c != '\0')
        {
            c = next();
        }
    }
}

Token LithiumTokenizer::getToken()
{
    text.clear();

    char c = peek();

    // skip whitespace and comments
    while (isWhitespace(c) || c == '#')
    {
        skipComments();
        skipWhitespace();
        c = peek();
    }

    // check for end of file
    if (c == '\0')
    {
        return {END, "", getLocation()};
    }

    // calculate token start location
    Location tokenStart = getLocation();

    // parse two character tokens
    if (c == '=')
    {
        char c2 = next();
        if (c2 == '=')
        {
            text = "==";
            next();
            return {EQUAL, text, tokenStart};
        }

        text = c;
        return {c, text, tokenStart};
    }
    else if (c == '<')
    {
        char c2 = next();
        if (c2 == '=')
        {
            text = "<=";
            next();
            return {LESS_OR_EQUAL, text, tokenStart};
        }

        text = c;
        return {c, text, tokenStart};
    }
    else if (c == '>')
    {
        char c2 = next();
        if (c2 == '=')
        {
            text = ">=";
            next();
            return {GREATER_OR_EQUAL, text, tokenStart};
        }

        text = c;
        return {c, text, tokenStart};
    }
    else if (c == '!')
    {
        char c2 = next();
        if (c2 == '=')
        {
            text = "!=";
            next();
            return {NOT_EQUAL, text, tokenStart};
        }

        text = c;
        return {c, text, tokenStart};
    }

    // parse single character tokens
    if (c == ';' ||
        c == '(' ||
        c == ')' ||
        c == '{' ||
        c == '}' ||
        c == '*' ||
        c == '/' ||
        c == '%' ||
        c == '^' ||
        c == '<' ||
        c == '>' ||
        c == ',')
    {
        text = c;
        next();
        return {c, text, tokenStart};
    }

    if (c == '+')
    {
        text = c;
        c = next();
        if (c == '+')
        {
            text += c;
            next();
            return {INCREMENT, text, tokenStart};
        }

        return {'+', text, tokenStart};
    }

    if (c == '-')
    {
        text = c;
        c = next();
        if (c == '-')
        {
            text += c;
            next();
            return {DECREMENT, text, tokenStart};
        }

        return {'-', text, tokenStart};
    }

    if (c == '|')
    {
        text = c;
        c = next();
        if (c == '|')
        {
            text += c;
            next();
            return {OR, text, tokenStart};
        }

        return {JUNK, text, tokenStart};
    }

    if (c == '&')
    {
        text = c;
        c = next();
        if (c == '&')
        {
            text += c;
            next();
            return {AND, text, tokenStart};
        }

        return {JUNK, text, tokenStart};
    }

    // parse strings
    if (c == '"' || c == '\'')
    {
        char quote = c;
        c = next();
        while (c != quote && c != '\0')
        {
            text += c;
            c = next();
        }

        if (c == '\0')
        {
            Token junk(JUNK, text, tokenStart);
            error("unterminated string", junk);
            return {JUNK, text, tokenStart};
        }

        next();
        return {STRING, text, tokenStart};
    }

    // parse numbers
    if (isdigit(c))
    {
        bool hasDot = false;
        while (isdigit(c) || (c == '.' && !hasDot))
        {
            if (c == '.')
            {
                hasDot = true;
            }

            text += c;
            c = next();
        }

        return {NUMBER, text, tokenStart};
    }

    // parse identifiers
    if (isalpha(c) || c == '_')
    {
        while (isalnum(c) || c == '_')
        {
            text += c;
            c = next();
        }

        if (text == "print")
        {
            return {PRINT, text, tokenStart};
        }

        if (text == "let")
        {
            return {LET, text, tokenStart};
        }

        if (text == "const")
        {
            return {LET, text, tokenStart};
        }

        if (text == "fn")
        {
            return {FN, text, tokenStart};
        }

        if (text == "return")
        {
            return {RETURN, text, tokenStart};
        }

        if (text == "asm")
        {
            return {ASM, text, tokenStart};
        }

        if (text == "for")
        {
            return {FOR, text, tokenStart};
        }

        if (text == "while")
        {
            return {WHILE, text, tokenStart};
        }

        if (text == "if")
        {
            return {IF, text, tokenStart};
        }

        if (text == "else")
        {
            return {ELSE, text, tokenStart};
        }

        return {IDENTIFIER, text, tokenStart};
    }

    // consume junk token
    text = c;
    next();
    return {JUNK, text, tokenStart};
}

char LithiumTokenizer::peek() const
{
    if (index >= source.size())
    {
        return '\0';
    }

    return source[index];
}

char LithiumTokenizer::next()
{
    if (index >= source.size())
    {
        return '\0';
    }

    return source[++index];
}

Location LithiumTokenizer::getLocation() const
{
    Location result(1, 1, filename);

    for (size_t i = 0; i < index; i++)
    {
        if (source[i] == '\n')
        {
            result.incrementLine();
            result.setColumn(1);
        }
        else
        {
            result.incrementColumn();
        }
    }

    return result;
}
