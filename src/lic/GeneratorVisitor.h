#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <cstdint>

#include "IVisitor.h"
#include "Symbol.h"

class GeneratorVisitor : public IVisitor
{
public:
    GeneratorVisitor(const std::string &outputFilename = "");
    ~GeneratorVisitor();

    virtual void visitAllChildren(Node* node) override;

    virtual void visit(ProgramNode* node) override;
    virtual void visit(BinaryExpressionNode *node) override;
    virtual void visit(IntExpressionNode *node) override;
    virtual void visit(PrintStatementNode* node) override;
    virtual void visit(UnaryExpressionNode* node) override;
    virtual void visit(AsmStatementNode *node) override;
    virtual void visit(StringExpressionNode* node) override;
    virtual void visit(ConcatNode* node) override;
    virtual void visit(VarDeclNode* node) override;
    virtual void visit(VariableExpressionNode* node) override;
    virtual void visit(AssignNode* node) override;
    virtual void visit(ForStatementNode* node) override;
    virtual void visit(WhileStatementNode* node) override;

    std::string getOutput() const;
    bool hasError() const;
private:
    void out(const std::string& text);
    void out(int64_t value);
private:
    std::string _outputFilename;
    std::stringstream _buffer;
    std::unordered_map<Symbol *, uint64_t> _variables;
    bool hadError = false;
};