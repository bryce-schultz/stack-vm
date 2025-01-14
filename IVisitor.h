#pragma once

class Node;
class ProgramNode;
class StatementNode;
class ExpressionNode;
class BinaryExpressionNode;
class OperatorNode;
class IntExpressionNode;
class UnaryExpressionNode;
class PrintStatementNode;

class IVisitor
{
public:
	virtual void visitAllChildren(Node* node) = 0;
	virtual void visit(ProgramNode* node);
	virtual void visit(StatementNode* node);
	virtual void visit(ExpressionNode* node);
	virtual void visit(BinaryExpressionNode* node);
	virtual void visit(OperatorNode* node);
	virtual void visit(IntExpressionNode* node);
	virtual void visit(UnaryExpressionNode* node);
	virtual void visit(PrintStatementNode* node);
};

