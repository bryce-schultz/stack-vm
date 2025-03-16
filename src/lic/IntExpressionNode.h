#pragma once

#include "NumericExpressionNode.h"
#include "IVisitor.h"

class IntExpressionNode : public NumericExpressionNode
{
public:
	IntExpressionNode(int value) : value(value) {}

	virtual void visit(IVisitor* visitor) override
	{
		visitor->visit(this);
	}

	int getValue() const { return value; }
private:
	int value;
};

