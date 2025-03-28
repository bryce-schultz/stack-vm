//***********************************************
// IfStatementNode.h
//***********************************************

#pragma once

#include "StatementNode.h"
#include "NumericExpressionNode.h"
#include "IVisitor.h"

class IfStatementNode : public StatementNode
{
public:
    IfStatementNode(NumericExpressionNode *condition, StatementNode *statement, StatementNode *elseStatement = nullptr)
    {
        addChild(condition);
        addChild(statement);
        if (elseStatement)
        {
            addChild(elseStatement);
        }
    }

    NumericExpressionNode *getCondition() const
    {
        return static_cast<NumericExpressionNode *>(getChild(0));
    }

    StatementNode *getStatement() const
    {
        return static_cast<StatementNode *>(getChild(1));
    }

    StatementNode *getElseStatement() const
    {
        if (getChildCount() == 3)
        {
            return static_cast<StatementNode *>(getChild(2));
        }

        return nullptr;
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
};