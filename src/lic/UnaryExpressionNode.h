//***********************************************
// UnaryExpressionNode.h
//***********************************************

#pragma once

#include "ExpressionNode.h"
#include "OperatorNode.h"

class UnaryExpressionNode : public ExpressionNode
{
public:
	UnaryExpressionNode(ExpressionNode *expr, Token op)
	{
		addChild(expr);
		addChild(new OperatorNode(op));
	}

	ExpressionNode *getExpr() const
	{
		return dynamic_cast<ExpressionNode *>(getChild(0));
	}

	OperatorNode *getOperator() const
	{
		return dynamic_cast<OperatorNode *>(getChild(1));
	}

	virtual void visit(IVisitor *visitor) override
	{
		visitor->visit(this);
	}

	virtual const Token getToken() const override
	{
		std::string exprText = getExpr()->getToken().getText();
		std::string opText = getOperator()->getToken().getText();
		int tokenType = getExpr()->getToken().getType();
		Location tokenLocation = getExpr()->getToken().getLocation();
		Token newToken(tokenType, exprText + opText, tokenLocation);
		return newToken;
	}

	virtual bool isString() const override
	{
		return getExpr()->isString();
	}

	virtual bool isNumeric() const override
	{
		return getExpr()->isNumeric();
	}
};