//***********************************************
// ParamNode.h
//***********************************************

#pragma once

#include "Symbol.h"
#include "DeclNode.h"
#include "LithiumSymbolTable.h"

class ParamNode : public DeclNode
{
public:
    ParamNode(const Token &identifier)
    {
        symbol = new Symbol(identifier);
        symbol->setDecl(this);
        global::symbolTable.addSymbol(symbol);
    }

    Symbol *getSymbol() const
    {
        return symbol;
    }
    
    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
private:
    Symbol *symbol;
};