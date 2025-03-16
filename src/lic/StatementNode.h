#pragma once

#include "Node.h"

class StatementNode : public Node
{
public:
	virtual void visit(IVisitor* visitor) override
	{
		visitor->visit(this);
	}
};