#pragma once

#include "DeclNode.h"
#include "Symbol.h"
#include "LithiumSymbolTable.h"
#include "ParamListNode.h"

class FuncDeclNode : public DeclNode
{
public:
    FuncDeclNode(const Token &token, ParamListNode *params, StatementNode *body)
    {
        if (global::symbolTable.lookupLocal(token.getText()))
        {
            // if the variable is already declared in the current scope reassing the symbol
            symbol = global::symbolTable.lookupLocal(token.getText());
        }
        else
        {
            symbol = new Symbol(token);
            symbol->setDecl(this);
            global::symbolTable.addSymbol(symbol);
        }

        addChild(params);
        addChild(body);
    }

    virtual bool isFunction() const override
    {
        return true;
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }

    ParamListNode *getParamList() const
    {
        return static_cast<ParamListNode *>(getChild(0));
    }

    StatementNode *getBody() const
    {
        return static_cast<StatementNode *>(getChild(1));
    }

    Symbol *getSymbol() const
    {
        return symbol;
    }

private:
    Symbol *symbol;
};