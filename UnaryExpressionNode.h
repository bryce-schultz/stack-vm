#pragma once

#include "Node.h"
#include "ExpressionNode.h"
#include "OperatorNode.h"

class UnaryExpressionNode : public Node
{
public:
	UnaryExpressionNode(ExpressionNode* expr, char op)
	{
		addChild(expr);
		addChild(new OperatorNode(op));
	}

	ExpressionNode* getExpr() const
	{
		return dynamic_cast<ExpressionNode*>(getChild(0));
	}

	char getOperator() const
	{
		return dynamic_cast<OperatorNode*>(getChild(1))->getOperator();
	}

	virtual void visit(IVisitor* visitor) override
	{
		visitor->visit(this);
	}
};

