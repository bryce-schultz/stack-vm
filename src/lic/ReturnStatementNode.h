//***********************************************
// ReturnStatementNode.h
//***********************************************

#pragma once

#include "StatementNode.h"
#include "ExpressionNode.h"

class ReturnStatementNode : public StatementNode 
{
public:
    ReturnStatementNode(ExpressionNode* expression = nullptr)
    {
        if (expression) 
        {
            addChild(expression);
        }
    }

    ExpressionNode* getExpression() const 
    {
        return dynamic_cast<ExpressionNode*>(getChild(0));
    }

    virtual void visit(IVisitor* visitor) override 
    {
        visitor->visit(this);
    }
};