#pragma once

#include "StringExpressionNode.h"

class ConcatNode : public StringExpressionNode
{
public:
    ConcatNode(StringExpressionNode *left, StringExpressionNode *right):
        StringExpressionNode(left->getValue() + right->getValue())
    {
        addChild(left);
        addChild(right);
    }

    StringExpressionNode *getLeft() const
    {
        return dynamic_cast<StringExpressionNode *>(getChild(0));
    }

    StringExpressionNode *getRight() const
    {
        return dynamic_cast<StringExpressionNode *>(getChild(1));
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
};