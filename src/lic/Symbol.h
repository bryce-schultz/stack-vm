#pragma once

#include "Token.h"
#include "DeclNode.h"

class Symbol : public Node
{
public:
    Symbol(const Token &token):
        token(token),
        decl(nullptr)
    {
    }

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

    bool isFunction() const
    {
        return decl && decl->isFunction();
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
private:
    Token token;
    DeclNode *decl;
};
