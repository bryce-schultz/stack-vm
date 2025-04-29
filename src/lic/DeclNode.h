//***********************************************
// DeclNode.h
//***********************************************

#pragma once

#include "StatementNode.h"

class DeclNode : public StatementNode
{
public:
    virtual bool isFunction() const
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
protected:
    Symbol *symbol;
};