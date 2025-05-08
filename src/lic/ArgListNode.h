//***********************************************
// ArgListNode.h
//***********************************************

#pragma once

#include "Node.h"
#include "ArgNode.h"
#include "IVisitor.h"
#include "Error.h"

class ArgListNode : public Node
{
public:
    ArgListNode(ArgNode *arg = nullptr)
    {
        addArg(arg);
    }

    void addArg(ArgNode *arg)
    {
        if (!arg)
        {
            return;
        }
        
        if (arg->isString())
        {
            error("string arguments are not allowed", arg->getToken());
            return;
        }

        addChild(arg);
    }

    void addAllArgs(ArgListNode *argList)
    {
        for (int i = 0; i < argList->getChildCount(); ++i)
        {
            addChild(argList->getChild(i));
        }
    }

    ArgNode *getArg(int index) const
    {
        return static_cast<ArgNode *>(getChild(index));
    }

    int getArgCount() const
    {
        return getChildCount();
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
};