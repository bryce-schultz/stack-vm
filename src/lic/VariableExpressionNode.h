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

    virtual bool isVariable() const override
    {
        return true;
    }

    const std::string getName() const
    {
        return token.getText();
    }

    Token getToken() const
    {
        return token;
    }

    void setSymbol(Symbol *symbol)
    {
        this->symbol = symbol;
    }

    Symbol *getSymbol() const
    {
        return symbol;
    }
private:
    Symbol *symbol;
    Token token;
};