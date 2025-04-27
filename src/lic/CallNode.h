#pragma once

#include "NumericExpressionNode.h"
#include "ArgListNode.h"
#include "Symbol.h"
#include "LithiumSymbolTable.h"

class CallNode : public NumericExpressionNode
{
public:
    CallNode(const Token &identifier, ArgListNode *args)
    {
        symbol = global::symbolTable.lookupGlobal(identifier.getText());
        if (!symbol)
        {
            undefined_function(identifier);
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

private:
    Symbol *symbol;
};