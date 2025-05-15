//***********************************************
// BinaryExpressionNode.h
//***********************************************

#pragma once

#include "ExpressionNode.h"
#include "OperatorNode.h"

class BinaryExpressionNode : public ExpressionNode
{
public:
	BinaryExpressionNode(ExpressionNode *left, Token op, ExpressionNode *right)
	{
		addChild(left);
		addChild(new OperatorNode(op));
		addChild(right);
	}

	ExpressionNode *getLeft() const
	{
		return dynamic_cast<ExpressionNode *>(getChild(0));
	}

	ExpressionNode *getRight() const
	{
		return dynamic_cast<ExpressionNode *>(getChild(2));
	}

	OperatorNode* getOperator() const
	{
		return dynamic_cast<OperatorNode*>(getChild(1));
	}

	virtual bool isString() const override
	{
		return getLeft()->isString() || getRight()->isString();
	}

	virtual bool isNumeric() const override
	{
		return getLeft()->isNumeric() && getRight()->isNumeric();
	}

	virtual const Token getToken() const override
	{
		return getLeft()->getToken();
	}

	virtual void visit(IVisitor* visitor) override
	{
		visitor->visit(this);
	}
};
