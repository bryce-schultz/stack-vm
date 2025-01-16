#pragma once
#include <string>
#include <fstream>
#include <sstream>

#include "../include/IVisitor.h"

class GeneratorVisitor : public IVisitor
{
public:
	GeneratorVisitor(const std::string &output_filename = "");
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

	std::string getOutput() const;
private:
	void out(const std::string& text);
	void out(int64_t value);
private:
	std::string _output_filename;
	std::stringstream _buffer;
};

