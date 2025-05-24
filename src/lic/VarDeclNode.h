//***********************************************
// VarDeclNode.h
//***********************************************

#pragma once

#include "DeclNode.h"
#include "Symbol.h"
#include "LithiumSymbolTable.h"
#include "Error.h"

class VarDeclNode : public DeclNode
{
public:
    VarDeclNode(const Token &identifier, ExpressionNode *expression, bool isConst = false):
        _isConst(isConst)
    {
        symbol = global::symbolTable.lookupLocal(identifier.getText());
        if (symbol)
        {
            std::string type = symbol->isFunction() ? "function" : symbol->isConst() ? "constant" : "variable";
            error(type + " '" + identifier.getText() + "' already declared in this scope", identifier);
            return;
        }

        symbol = new Symbol(identifier);
        symbol->setDecl(this);
        global::symbolTable.addSymbol(symbol);

        addChild(expression);
    }

    virtual Type getType() const override
    {
        return getExpression()->getType();
    }

    virtual bool isConst() const override
    {
        return _isConst;
    }

    std::string getName() const
    {
        return symbol->getName();
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }

    ExpressionNode *getExpression() const
    {
        return static_cast<ExpressionNode *>(getChild(0));
    }
private:
    bool _isConst;
};