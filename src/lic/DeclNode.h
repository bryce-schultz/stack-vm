//***********************************************
// DeclNode.h
//***********************************************

#pragma once

#include "StatementNode.h"

class DeclNode : public StatementNode
{
public:
    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
};