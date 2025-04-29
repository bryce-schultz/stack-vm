//***********************************************
// OperatorNode.h
//***********************************************

#pragma once

#include "IVisitor.h"
#include "Node.h"

class OperatorNode : public Node
{
public:
	OperatorNode(int op) : op(op) {}
	int getOperator() const { return op; }

	virtual void visit(IVisitor *visitor) override
	{
		visitor->visit(this);
	}
private:
	int op;
};

