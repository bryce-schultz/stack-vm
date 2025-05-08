#include "StringExpressionNode.h"
#include "NumericExpressionNode.h"

class StringConversionNode : public StringExpressionNode
{
public:
    StringConversionNode(NumericExpressionNode *expression)
        : StringExpressionNode(expression->getToken())
    {
        addChild(expression);
    }
    
    NumericExpressionNode *getExpression() const
    {
        return static_cast<NumericExpressionNode *>(getChild(0));
    }

    virtual void visit(IVisitor *visitor) override
    {
        visitor->visit(this);
    }
};
