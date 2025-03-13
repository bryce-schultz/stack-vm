#pragma once

#include "DeclNode.h"
#include "Symbol.h"
#include "LithiumSymbolTable.h"

class VarDeclNode : public DeclNode
{
public:
    VarDeclNode(const Token &token, ExpressionNode *expression)
    {
        symbol = new Symbol(token);
        addChild(expression);
        // add the symbol to the symbol table
        symbol->setDecl(this);
        global::symbolTable.addSymbol(symbol);
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