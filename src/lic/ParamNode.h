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

    const Token getToken() const override
    {
        return symbol->getToken();
    }

    const std::string getName() const
    {
        return symbol->getName();
    }
    
    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
private:
    Symbol *symbol;
};