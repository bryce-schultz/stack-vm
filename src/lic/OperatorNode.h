//***********************************************
// OperatorNode.h
//***********************************************

#pragma once

#include "IVisitor.h"
#include "Node.h"

class OperatorNode : public Node
{
public:
	OperatorNode(Token token):
		value(token.getType()),
		token(token)
	{
	}

	int getValue() const { return value; }

	virtual const Token getToken() const override
	{
		return token;
	}

	virtual void visit(IVisitor *visitor) override
	{
		visitor->visit(this);
	}
private:
	int value;
	Token token;
};

