//***********************************************
// UnaryExpressionNode.h
//***********************************************

#pragma once

#include "ExpressionNode.h"
#include "OperatorNode.h"

class UnaryExpressionNode : public ExpressionNode
{
public:
	UnaryExpressionNode(ExpressionNode *expr, int op)
	{
		addChild(expr);
		addChild(new OperatorNode(op));
	}

	ExpressionNode *getExpr() const
	{
		return dynamic_cast<ExpressionNode *>(getChild(0));
	}

	int getOperator() const
	{
		return dynamic_cast<OperatorNode *>(getChild(1))->getOperator();
	}

	virtual void visit(IVisitor *visitor) override
	{
		visitor->visit(this);
	}
};