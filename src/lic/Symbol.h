//***********************************************
// Symbol.h
//***********************************************

#pragma once

#include "Token.h"
#include "DeclNode.h"
#include "Type.h"

class Symbol : public Node
{
public:
    Symbol(const Token &token):
        token(token),
        decl(nullptr)
    {
    }

    Type getType() const
    {
        if (decl)
        {
            return decl->getType();
        }

        return Type::UNDEFINED;
    }

    DeclNode *getDecl() const
    {
        return decl;
    }

    void setDecl(DeclNode *decl)
    {
        this->decl = decl;
    }

    virtual const Token getToken() const override
    {
        return token;
    }

    bool isConst() const
    {
        return decl && decl->isConst();
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
