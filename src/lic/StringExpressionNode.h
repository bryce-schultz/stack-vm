//***********************************************
// StringExpressionNode.h
//***********************************************

#pragma once

#include <string>

#include "ExpressionNode.h"
#include "Token.h"

class StringExpressionNode : public ExpressionNode
{
public:
    StringExpressionNode(const Token &token): 
        token(token)
    {
    }

    const std::string &getValue() const
    {
        return token.getText();
    }

    virtual const Token getToken() const override
    {
        return token;
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }

    virtual bool isString() const override
    {
        return true;
    }
private:
    Token token;
};