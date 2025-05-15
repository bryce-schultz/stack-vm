//***********************************************
// NumberExpressionNode.h
//***********************************************

#pragma once

#include "NumericExpressionNode.h"
#include "IVisitor.h"

class NumberExpressionNode : public NumericExpressionNode
{
public:
	NumberExpressionNode(double value) : value(value) {}

	virtual void visit(IVisitor* visitor) override
	{
		visitor->visit(this);
	}

	bool isInteger() const
	{
		return value == static_cast<int>(value);
	}

	double getValue() const { return value; }
	int getValueAsInt() const
	{
		return static_cast<int>(value);
	}
private:
	double value;
};

