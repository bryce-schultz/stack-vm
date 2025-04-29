//***********************************************
// ParamListNode.h
//***********************************************

#pragma once

#include "Node.h"
#include "ParamNode.h"
#include "IVisitor.h"

class ParamListNode : public Node
{
public:
    ParamListNode(ParamNode *param = nullptr)
    {
        if (param)
        {
            addChild(param);
        }
    }

    void addParam(ParamNode *param)
    {
        addChild(param);
    }

    void addAllParams(ParamListNode *paramList)
    {
        for (int i = 0; i < paramList->getChildCount(); ++i)
        {
            addChild(paramList->getChild(i));
        }
    }

    ParamNode *getParam(int index) const
    {
        return static_cast<ParamNode *>(getChild(index));
    }

    int getParamCount() const
    {
        return getChildCount();
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
};