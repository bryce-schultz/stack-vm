//***********************************************
// Token.cpp
//***********************************************

#include "Token.h"

Token::Token(int type, const std::string &text, const Location &location):
    type(type),
    text(text),
    location(location)
{
}

bool Token::operator==(const Token &other) const
{
    return type == other.type && text == other.text && location == other.location;
}

bool Token::operator==(int type) const
{
    return this->type == type;
}

bool Token::operator==(char type) const
{
    return this->type == type;
}

bool Token::operator!=(const Token &other) const
{
    return !(*this == other);
}

bool Token::operator!=(int type) const
{
    return !(*this == type);
}

bool Token::operator!=(char type) const
{
    return !(*this == type);
}

Token::operator int() const
{
    return type;
}

int Token::getType() const
{
    return type;
}

const std::string &Token::getText() const
{
    return text;
}

const Location &Token::getLocation() const
{
    return location;
}

size_t Token::getLength() const
{
    return text.length();
}

std::string tokenTypeToString(int type)
{
    switch (type)
    {
        case NUMBER:
            return "NUMBER";
        case PRINT:
            return "PRINT";
        case IDENTIFIER:
            return "IDENTIFIER";
        case STRING:
            return "STRING";
        case ASM:
            return "ASM";
        case END:
            return "END";
        case JUNK:
            return "JUNK";
        case LESS_OR_EQUAL:
            return "<=";
        case GREATER_OR_EQUAL:
            return ">=";
        case EQUAL:
            return "==";
        case NOT_EQUAL:
            return "!=";
        case OR:
            return "||";
        case AND:
            return "&&";
        case INCREMENT:
            return "++";
        case DECREMENT:
            return "--";
        case FOR:
            return "FOR";
        case WHILE:
            return "WHILE";
        case LET:
            return "LET";
        case FN:
            return "FN";
        case RETURN:
            return "RETURN";
        case IF:
            return "IF";
        case ELSE:
            return "ELSE";
        case STR:
            return "STR";
        default:
        {
            if (type >= 0 && type <= 255)
            {
                return std::string(1, type);
            }

            return "UNKNOWN";
        }
    }
}
