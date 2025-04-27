#pragma once

#include "Node.h"
#include "IVisitor.h"

class ArgNode : public Node
{
public:
    ArgNode(Node *expression)
    {
        addChild(expression);
    }

    Node *getExpression() const
    {
        return getChild(0);
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
};