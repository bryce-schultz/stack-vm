#pragma once

#include "NumericExpressionNode.h"
#include "Token.h"

class VariableExpressionNode : public NumericExpressionNode
{
public:
    VariableExpressionNode(Token token):
        symbol(nullptr),
        token(token)
    {
        symbol = global::symbolTable.lookupGlobal(token.getText());
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }

    const std::string getName() const
    {
        return token.getText();
    }

    Token getToken() const
    {
        return token;
    }

    Symbol *getSymbol() const
    {
        return symbol;
    }
private:
    Symbol *symbol;
    Token token;
};