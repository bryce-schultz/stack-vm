//***********************************************
// StatementsNode.h
//***********************************************

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
        if (statement)
        {
            addChild(statement);
        }
    }

    StatementNode *getStatement(int index) const
    {
        if (index >= getChildCount())
        {
            return nullptr;
        }
        return static_cast<StatementNode *>(getChild(index));
    }

    int getStatementCount() const
    {
        return getChildCount();
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