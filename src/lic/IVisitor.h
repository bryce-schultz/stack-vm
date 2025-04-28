//***********************************************
// IVisitor.h
//***********************************************

#pragma once

class Node;
class ProgramNode;
class StatementNode;
class ExpressionNode;
class BinaryExpressionNode;
class OperatorNode;
class UnaryExpressionNode;
class PrintStatementNode;
class AsmStatementNode;
class NumericExpressionNode;
class StringExpressionNode;
class IntExpressionNode;
class ConcatNode;
class DeclNode;
class VarDeclNode;
class VariableExpressionNode;
class AssignNode;
class StatementsNode;
class ForStatementNode;
class WhileStatementNode;
class IfStatementNode;
class BlockNode;
class StatementListNode;
class FuncDeclNode;
class ParamNode;
class ParamListNode;
class ArgNode;
class ArgListNode;
class CallNode;
class Symbol;
class ReturnStatementNode;

class IVisitor
{
public:
	virtual void visitAllChildren(Node* node) = 0;
	virtual void visit(ProgramNode* node);
	virtual void visit(StatementNode* node);
	virtual void visit(ExpressionNode* node);
	virtual void visit(BinaryExpressionNode* node);
	virtual void visit(OperatorNode* node);
	virtual void visit(UnaryExpressionNode* node);
	virtual void visit(PrintStatementNode* node);
	virtual void visit(AsmStatementNode* node);
	virtual void visit(NumericExpressionNode* node);
	virtual void visit(StringExpressionNode* node);
	virtual void visit(IntExpressionNode* node);
	virtual void visit(ConcatNode* node);
	virtual void visit(DeclNode* node);
	virtual void visit(VarDeclNode* node);
	virtual void visit(VariableExpressionNode* node);
	virtual void visit(AssignNode* node);
	virtual void visit(StatementsNode* node);
	virtual void visit(ForStatementNode* node);
	virtual void visit(WhileStatementNode* node);
	virtual void visit(IfStatementNode* node);
	virtual void visit(BlockNode* node);
	virtual void visit(StatementListNode* node);
	virtual void visit(FuncDeclNode* node);
	virtual void visit(ParamNode* node);
	virtual void visit(ParamListNode* node);
	virtual void visit(ArgNode* node);
	virtual void visit(ArgListNode* node);
	virtual void visit(CallNode* node);
	virtual void visit(Symbol* node);
	virtual void visit(ReturnStatementNode* node);
};

