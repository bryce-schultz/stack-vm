#pragma once

#include "Node.h"

class ExpressionNode : public Node
{
public:
	virtual void visit(IVisitor* visitor) override
	{
		visitor->visit(this);
	}

	virtual bool isNumeric() const
	{
		return false;
	}

	virtual bool isString() const
	{
		return false;
	}
};

