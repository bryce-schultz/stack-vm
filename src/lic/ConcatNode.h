#pragma once

#include "StringExpressionNode.h"

class ConcatNode : public StringExpressionNode
{
public:
    ConcatNode(StringExpressionNode *left, ExpressionNode *right)
    {
        addChild(left);
        addChild(right);
    }

    StringExpressionNode *getLeft() const
    {
        return dynamic_cast<StringExpressionNode *>(getChild(0));
    }

    ExpressionNode *getRight() const
    {
        return dynamic_cast<ExpressionNode *>(getChild(1));
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
};