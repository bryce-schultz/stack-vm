//***********************************************
// NumericExpressionNode.h
//***********************************************

#pragma once

#include "ExpressionNode.h"

class NumericExpressionNode : public ExpressionNode
{
public:
	virtual void visit(IVisitor* visitor) override
	{
		visitor->visit(this);
	}

    virtual bool isNumeric() const override
    {
        return true;
    }
};