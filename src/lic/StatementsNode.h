#pragma once

#include "StatementNode.h"

class StatementsNode : public Node
{
public:
    StatementsNode(StatementNode *statement = nullptr)
    {
        if (statement)
        {
            addChild(statement);
        }
    }

    void addStatement(StatementNode *statement)
    {
        addChild(statement);
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }

    bool empty() const
    {
        return getChildCount() == 0;
    }
};