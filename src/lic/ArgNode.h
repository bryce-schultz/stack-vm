//***********************************************
// ArgNode.h
//***********************************************

#pragma once

#include "ExpressionNode.h"
#include "IVisitor.h"

class ArgNode : public ExpressionNode
{
public:
    ArgNode(ExpressionNode *expression)
    {
        addChild(expression);
    }

    ExpressionNode *getExpression() const
    {
        return static_cast<ExpressionNode *>(getChild(0));
    }

    virtual bool isString() const override
    {
        return getExpression()->isString();
    }

    virtual const Token getToken() const override
    {
        return getExpression()->getToken();
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
};