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

	StatementsNode *getStatements() const
	{
		return static_cast<StatementsNode *>(getChild(0));
	}

	virtual void visit(IVisitor *visitor) override
	{
		visitor->visit(this);
	}
};
