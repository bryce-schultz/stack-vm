//***********************************************
// PrintStatementNode.h
//***********************************************

#pragma once

#include "StatementNode.h"
#include "ExpressionNode.h"

class PrintStatementNode : public StatementNode
{
public:
	PrintStatementNode(ExpressionNode* expression)
	{
		addChild(expression);
	}

	virtual void visit(IVisitor* visitor) override
	{
		visitor->visit(this);
	}

	ExpressionNode* getExpression() const
	{
		return dynamic_cast<ExpressionNode*>(getChild(0));
	}
};

