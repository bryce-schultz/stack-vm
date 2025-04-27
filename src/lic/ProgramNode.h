#pragma once

#include "Node.h"
#include "StatementsNode.h"
#include "HasVisitor.h"

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

	bool hasMain()
	{
		HasVisitor visitor;
		visitAllChildren(&visitor);

		return visitor.has("main");
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
