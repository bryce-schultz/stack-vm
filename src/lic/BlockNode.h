//***********************************************
// BlockNode.h
//***********************************************

#pragma once

#include "StatementsNode.h"
#include "StatementNode.h"

class BlockNode : public StatementNode
{
public:
    BlockNode(StatementsNode *statements)
    {
        addChild(statements);
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
};