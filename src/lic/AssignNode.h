#pragma once

#include "StatementNode.h"
#include "ExpressionNode.h"
#include "Token.h"
#include "LithiumSymbolTable.h"

class AssignNode : public StatementNode
{
public:
    AssignNode(const Token &token, ExpressionNode *expression)
    {
        symbol = global::symbolTable.lookupGlobal(token.getText());
        if (!symbol)
        {
            symbol = global::symbolTable.addSymbol(new Symbol(token));
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