#pragma once

#include "../../visitors/include/IVisitor.h"
#include "Node.h"

class OperatorNode : public Node
{
public:
	OperatorNode(char op) : op(op) {}
	char getOperator() const { return op; }

	virtual void visit(IVisitor *visitor) override
	{
		visitor->visit(this);
	}
private:
	char op;
};

