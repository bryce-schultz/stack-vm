#pragma once

#include "Node.h"
#include "StatementsNode.h"

class ProgramNode : public Node
{
public:
	ProgramNode(StatementsNode *statements = nullptr)
	{
		if (statements)
		{
			addChild(statements);
		}
	}

	virtual void visit(IVisitor *visitor) override
	{
		visitor->visit(this);
	}
};
