#pragma once

#include "Node.h"

class ProgramNode : public Node
{
public:
	virtual void visit(IVisitor *visitor) override
	{
		visitor->visit(this);
	}
};

