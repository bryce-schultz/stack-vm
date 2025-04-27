#pragma once

#include "DeclNode.h"
#include "Symbol.h"
#include "LithiumSymbolTable.h"
#include "Error.h"

class VarDeclNode : public DeclNode
{
public:
    VarDeclNode(const Token &identifier, ExpressionNode *expression)
    {
        if (global::symbolTable.lookupLocal(identifier.getText()))
        {
            // if the variable is already declared in the current scope reassing the symbol
            symbol = global::symbolTable.lookupLocal(identifier.getText());
        }
        else
        {
            symbol = new Symbol(identifier);
            symbol->setDecl(this);
            global::symbolTable.addSymbol(symbol);
        }

        addChild(expression);
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }

    Symbol *getSymbol() const
    {
        return symbol;
    }

    ExpressionNode *getExpression() const
    {
        return static_cast<ExpressionNode *>(getChild(0));
    }
private:
    Symbol *symbol;
};