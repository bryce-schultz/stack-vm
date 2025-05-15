//***********************************************
// DeclNode.h
//***********************************************

#pragma once

#include "StatementNode.h"
#include "Type.h"

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

    virtual Type getType() const
    {
        return Type::UNDEFINED;
    }

    Symbol *getSymbol() const
    {
        return symbol;
    }

    virtual bool isDecl() const override
    {
        return true;
    }

    const Token getToken() const;
protected:
    Symbol *symbol;
};