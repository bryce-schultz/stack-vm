#pragma once

#include "Node.h"

class ExpressionNode : public Node
{
public:
	virtual void visit(IVisitor* visitor) override
	{
		visitor->visit(this);
	}
};

