//***********************************************
// AssignNode.h
//***********************************************

#pragma once

#include "StatementNode.h"
#include "ExpressionNode.h"
#include "Token.h"
#include "LithiumSymbolTable.h"
#include "IVisitor.h"
#include "NumericExpressionNode.h"

class AssignNode : public NumericExpressionNode
{
public:
    AssignNode(VariableExpressionNode *variable, ExpressionNode *expression)
    {
        symbol = global::symbolTable.lookupGlobal(variable->getToken().getText());
        if (!symbol)
        {
            symbol = global::symbolTable.addSymbol(new Symbol(variable->getToken()));
            if (!variable->getSymbol())
            {
                variable->setSymbol(symbol);
            }
        }
        addChild(expression);
    }

    Symbol *getSymbol() const
    {
        return symbol;
    }

    ExpressionNode *getExpression() const
    {
        return dynamic_cast<ExpressionNode *>(getChild(0));
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
private:
    Symbol *symbol;
};