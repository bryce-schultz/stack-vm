#pragma once

#include "DeclNode.h"
#include "Symbol.h"
#include "LithiumSymbolTable.h"
#include "Error.h"

class VarDeclNode : public DeclNode
{
public:
    VarDeclNode(const Token &token, ExpressionNode *expression)
    {
        if (global::symbolTable.lookupLocal(token.getText()))
        {
            // if the variable is already declared in the current scope reassing the symbol
            symbol = global::symbolTable.lookupLocal(token.getText());
        }
        else
        {
            printf("Declaring variable %s\n", token.getText().c_str());
            symbol = new Symbol(token);
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