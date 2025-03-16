#pragma once

#include "StatementNode.h"
#include "ExpressionNode.h"
#include "BlockNode.h"

class ForStatementNode : public StatementNode
{
public:
    ForStatementNode(StatementNode *init, ExpressionNode *condition, StatementNode *increment, BlockNode *block)
    {
        addChild(init);
        addChild(condition);
        addChild(increment);
        addChild(block);
    }

    StatementNode *getInit() const
    {
        return dynamic_cast<StatementNode *>(getChild(0));
    }

    ExpressionNode *getCondition() const
    {
        return dynamic_cast<ExpressionNode *>(getChild(1));
    }

    StatementNode *getIncrement() const
    {
        return dynamic_cast<StatementNode *>(getChild(2));
    }

    BlockNode *getBlock() const
    {
        return dynamic_cast<BlockNode *>(getChild(3));
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
};