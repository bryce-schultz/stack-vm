#pragma once

#include "StatementNode.h"
#include "ExpressionNode.h"
#include "StatementListNode.h"

class ForStatementNode : public StatementNode
{
public:
    ForStatementNode(StatementListNode *init, ExpressionNode *condition, StatementListNode *increment, StatementNode *statement)
    {
        addChild(init);
        addChild(condition);
        addChild(increment);
        addChild(statement);
    }

    StatementListNode *getInit() const
    {
        return dynamic_cast<StatementListNode *>(getChild(0));
    }

    ExpressionNode *getCondition() const
    {
        return dynamic_cast<ExpressionNode *>(getChild(1));
    }

    StatementListNode *getIncrement() const
    {
        return dynamic_cast<StatementListNode *>(getChild(2));
    }

    StatementNode *getStatement() const
    {
        return dynamic_cast<StatementNode *>(getChild(3));
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
};