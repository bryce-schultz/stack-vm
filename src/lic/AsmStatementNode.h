//***********************************************
// AsmStatementNode.h
//***********************************************

#pragma once

#include <string>

#include "StatementNode.h"
#include "ExpressionNode.h"

class AsmStatementNode : public StatementNode
{
public:
    AsmStatementNode(ExpressionNode *string_expression)
    {
        addChild(string_expression);
    }

    ExpressionNode *getExpression() const
    {
        return dynamic_cast<ExpressionNode *>(getChild(0));
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
};