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
        if (global::symbolTable.lookupLocal(identifier.getText()))
        {
            std::string type = isConst ? "const" : "variable";
            error(type + " '" + identifier.getText() + "' already declared in this scope", identifier);
            return;
        }
        else
        {
            symbol = new Symbol(identifier);
            symbol->setDecl(this);
            global::symbolTable.addSymbol(symbol);
        }

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