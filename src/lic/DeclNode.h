//***********************************************
// DeclNode.h
//***********************************************

#pragma once

#include "StatementNode.h"

class Symbol;

class DeclNode : public StatementNode
{
public:
    virtual bool isFunction() const
    {
        return false;
    }

    virtual bool isConst() const
    {
        return false;
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }

    Symbol *getSymbol() const
    {
        return symbol;
    }

    const Token getToken() const;
protected:
    Symbol *symbol;
};