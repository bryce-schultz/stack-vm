//***********************************************
// StatementListNode.h
//***********************************************

#pragma once

#include "Node.h"

class StatementListNode : public Node
{
public:
    StatementListNode(StatementNode *statement = nullptr)
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