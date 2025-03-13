#pragma once

#include "Token.h"
#include "DeclNode.h"

class Symbol
{
public:
    Symbol(const Token &token):
        token(token),
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

    Token getToken() const
    {
        return token;
    }

    std::string getName() const
    {
        return token.getText();
    }
private:
    Token token;
    DeclNode *decl;
};
