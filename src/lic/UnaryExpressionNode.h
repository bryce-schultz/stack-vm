#pragma once

#include "NumericExpressionNode.h"
#include "OperatorNode.h"

class UnaryExpressionNode : public NumericExpressionNode
{
public:
	UnaryExpressionNode(NumericExpressionNode *expr, char op)
	{
		addChild(expr);
		addChild(new OperatorNode(op));
	}

	NumericExpressionNode *getExpr() const
	{
		return dynamic_cast<NumericExpressionNode *>(getChild(0));
	}

	char getOperator() const
	{
		return dynamic_cast<OperatorNode *>(getChild(1))->getOperator();
	}

	virtual void visit(IVisitor *visitor) override
	{
		visitor->visit(this);
	}
};