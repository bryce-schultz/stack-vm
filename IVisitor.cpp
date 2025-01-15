#include "IVisitor.h"
#include "Nodes.h"

void IVisitor::visit(ProgramNode* node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(StatementNode* node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(ExpressionNode* node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(BinaryExpressionNode* node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(OperatorNode* node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(IntExpressionNode* node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(ConcatNode *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(UnaryExpressionNode* node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(PrintStatementNode* node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(AsmStatementNode *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(StringExpressionNode *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(NumericExpressionNode *node)
{
	node->visitAllChildren(this);
}
