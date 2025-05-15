//***********************************************
// FuncDeclNode.h
//***********************************************

#pragma once

#include "DeclNode.h"
#include "Symbol.h"
#include "LithiumSymbolTable.h"
#include "ParamListNode.h"

class FuncDeclNode : public DeclNode
{
public:
    FuncDeclNode(const Token &token, ParamListNode *params, StatementNode *body = nullptr)
    {
        symbol = new Symbol(token);
        symbol->setDecl(this);
        global::symbolTable.addSymbol(symbol);

        addChild(params);

        if (body)
        {
            addChild(body);
        }
    }

    void setParamList(ParamListNode *params)
    {
        setChild(0, params);
    }

    void setBody(StatementNode *body)
    {
        setChild(1, body);
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

    bool hasBody() const
    {
        return getBody() != nullptr;
    }

    Symbol *getSymbol() const
    {
        return symbol;
    }

    virtual const Token getToken() const override
    {
        return symbol->getToken();
    }

    const std::string getName() const
    {
        return symbol->getName();
    }

    virtual bool isFuncDecl() const override
    {
        return true;
    }
private:
    Symbol *symbol;
};