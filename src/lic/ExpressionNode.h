//***********************************************
// StatementNode.h
//***********************************************

#pragma once

#include "StatementNode.h"

class ExpressionNode : public StatementNode
{
public:
    virtual void visit(IVisitor* visitor) override
    {
        visitor->visit(this);
    }

    virtual bool isNumeric() const
    {
        return false;
    }

    virtual bool isString() const
    {
        return false;
    }

    virtual bool isVariable() const
    {
        return false;
    }
};

