//***********************************************
// IVisitor.cpp
//***********************************************

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

void IVisitor::visit(NumberExpressionNode* node)
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

void IVisitor::visit(DeclNode *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(VarDeclNode *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(VariableExpressionNode *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(AssignNode *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(StatementsNode *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(ForStatementNode *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(BlockNode *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(StatementListNode *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(WhileStatementNode *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(IfStatementNode *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(FuncDeclNode* node)
{
    node->visitAllChildren(this);
}

void IVisitor::visit(ParamNode *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(ParamListNode* node)
{
    node->visitAllChildren(this);
}

void IVisitor::visit(ArgNode* node)
{
    node->visitAllChildren(this);
}

void IVisitor::visit(ArgListNode* node)
{
    node->visitAllChildren(this);
}

void IVisitor::visit(CallNode* node) 
{
    node->visitAllChildren(this);
}

void IVisitor::visit(Symbol *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(ReturnStatementNode *node)
{
	node->visitAllChildren(this);
}

void IVisitor::visit(StringConversionNode *node)
{
	node->visitAllChildren(this);
}
