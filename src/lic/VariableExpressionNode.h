//***********************************************
// VariableExpressionNode.h
//***********************************************

#pragma once

#include "NumericExpressionNode.h"
#include "Token.h"

class VariableExpressionNode : public NumericExpressionNode
{
public:
    VariableExpressionNode(const Token &token):
        symbol(nullptr),
        token(token)
    {
        auto text = token.getText();
        symbol = global::symbolTable.lookupGlobal(text);

        if (!symbol)
        {
            undefined(token);
        }
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