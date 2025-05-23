//***********************************************
// CallNode.h
//***********************************************

#pragma once

#include "ExpressionNode.h"
#include "ArgListNode.h"
#include "Symbol.h"
#include "LithiumSymbolTable.h"

class CallNode : public ExpressionNode
{
public:
    CallNode(const Token &identifier, ArgListNode *args)
    {
        symbol = global::symbolTable.lookupGlobal(identifier.getText());
        if (!symbol)
        {
            undefined_function(identifier);
            return;
        }

        if (!symbol->isFunction())
        {
            error(identifier.getText() + " is not a function", identifier);
        }

        addChild(args);
    }

    ArgListNode *getArgs() const
    {
        return static_cast<ArgListNode *>(getChild(0));
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
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

    virtual bool isNumeric() const override
    {
        return true;
    }

private:
    Symbol *symbol;
};