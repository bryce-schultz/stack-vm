#pragma once

#include "DeclNode.h"
#include "Symbol.h"

class VarDeclNode : public DeclNode
{
public:
    VarDeclNode(Symbol *name)
    : name(name)
    {
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }

    Symbol *getName() const
    {
        return name;
    }

private:
    Symbol *name;
};