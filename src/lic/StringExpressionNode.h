#pragma once

#include <string>

#include "ExpressionNode.h"

class StringExpressionNode : public ExpressionNode
{
public:
    StringExpressionNode(const std::string &value = "") : value(value)
    {
    }

    const std::string &getValue() const
    {
        return value;
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }

    virtual bool isString() const override
    {
        return true;
    }

private:
    std::string value;
};