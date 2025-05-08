#include "StringExpressionNode.h"
#include "ExpressionNode.h"

class StringConversionNode : public ExpressionNode
{
public:
    StringConversionNode(ExpressionNode *expression)
    {
        addChild(expression);
    }
    
    ExpressionNode *getExpression() const
    {
        return static_cast<ExpressionNode *>(getChild(0));
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
};
