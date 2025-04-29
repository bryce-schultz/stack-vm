//***********************************************
// StatementNode.h
//***********************************************

#pragma once

#include "Node.h"

class StatementNode : public Node
{
public:
	virtual void visit(IVisitor* visitor) override
	{
		visitor->visit(this);
	}

	bool isEmpty() const
	{
		return _isEmpty;
	}

	void setEmpty(bool isEmpty)
	{
		_isEmpty = isEmpty;
	}
private:
	bool _isEmpty = false;
};