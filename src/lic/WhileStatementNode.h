//***********************************************
// WhileStatementNode.h
//***********************************************

#pragma once

#include "StatementNode.h"
#include "ExpressionNode.h"
#include "IVisitor.h"

class WhileStatementNode : public StatementNode
{
public:
    WhileStatementNode(ExpressionNode *condition, StatementNode *body)
    {
        addChild(condition);
        addChild(body);
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }

    ExpressionNode *getCondition() const
    {
        return static_cast<ExpressionNode *>(getChild(0));
    }

    StatementNode *getBody() const
    {
        return static_cast<StatementNode *>(getChild(1));
    }
};