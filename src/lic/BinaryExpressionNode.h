#pragma once

#include "NumericExpressionNode.h"
#include "OperatorNode.h"

class BinaryExpressionNode : public NumericExpressionNode
{
public:
	BinaryExpressionNode(NumericExpressionNode *left, int op, NumericExpressionNode *right)
	{
		addChild(left);
		addChild(new OperatorNode(op));
		addChild(right);
	}

	NumericExpressionNode *getLeft() const
	{
		return dynamic_cast<NumericExpressionNode *>(getChild(0));
	}

	NumericExpressionNode *getRight() const
	{
		return dynamic_cast<NumericExpressionNode *>(getChild(2));
	}

	int getOperator() const
	{
		return dynamic_cast<OperatorNode*>(getChild(1))->getOperator();
	}

	virtual void visit(IVisitor* visitor) override
	{
		visitor->visit(this);
	}
};
