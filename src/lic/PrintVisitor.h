#pragma once

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <fstream>

#include "IVisitor.h"
#include "Error.h"

class PrintVisitor : public IVisitor
{
public:
    PrintVisitor();
    void visitAllChildren(Node* node) override;
    void visit(ProgramNode* node) override;
    void visit(StatementNode* node) override;
    void visit(ExpressionNode* node) override;
    void visit(BinaryExpressionNode* node) override;
    void visit(OperatorNode* node) override;
    void visit(UnaryExpressionNode* node) override;
    void visit(PrintStatementNode* node) override;
    void visit(AsmStatementNode* node) override;
    void visit(NumericExpressionNode* node) override;
    void visit(StringExpressionNode* node) override;
    void visit(NumberExpressionNode* node) override;
    void visit(ConcatNode* node) override;
    void visit(DeclNode* node) override;
    void visit(VarDeclNode* node) override;
    void visit(VariableExpressionNode* node) override;
    void visit(AssignNode* node) override;
    void visit(StatementsNode* node) override;
    void visit(ForStatementNode* node) override;
    void visit(WhileStatementNode* node) override;
    void visit(IfStatementNode* node) override;
    void visit(BlockNode* node) override;
    void visit(StatementListNode* node) override;
    void visit(FuncDeclNode* node) override;
    void visit(ParamNode* node) override;
    void visit(ParamListNode* node) override;
    void visit(ArgNode* node) override;
    void visit(ArgListNode* node) override;
    void visit(CallNode* node) override;
    void visit(Symbol* node) override;
    void visit(ReturnStatementNode* node) override;
    void visit(StringConversionNode* node) override;

    std::string getOutput() const;

    void outputToFile(const std::string &filename);
private:
    void indent();
    void deindent();
    void openTag(const std::string &tag, const std::map<std::string, std::string> &attributes = {}, bool closeTag = false);
    void closeTag(const std::string &tag);
private:
    int indentLevel;
    std::stringstream outputBuffer;
};