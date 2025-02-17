#pragma once

#include "Token.h"
#include "DeclNode.h"

class Symbol
{
public:
    Symbol(std::string name, TokenType type):
        name(name),
        type(type),
        decl(nullptr)
    {}

    DeclNode *getDecl() const
    {
        return decl;
    }

    void setDecl(DeclNode *decl)
    {
        this->decl = decl;
    }
private:
    std::string name;
    TokenType type;
    DeclNode *decl;
};
