//***********************************************
// AsmStatementNode.h
//***********************************************

#pragma once

#include <string>

#include "StatementNode.h"
#include "StringExpressionNode.h"

class AsmStatementNode : public StatementNode
{
public:
    AsmStatementNode(StringExpressionNode *string_expression)
    {
        addChild(string_expression);
    }

    StringExpressionNode *getStringExpression() const
    {
        return dynamic_cast<StringExpressionNode *>(getChild(0));
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
};