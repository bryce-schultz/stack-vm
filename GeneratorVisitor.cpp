#include "GeneratorVisitor.h"
#include "Nodes.h"

GeneratorVisitor::GeneratorVisitor(const std::string &output_filename)
{
	_file.open(output_filename);
}

GeneratorVisitor::~GeneratorVisitor()
{
	if (_file.is_open())
	{
		_file.close();
	}
}

void GeneratorVisitor::visit(ProgramNode *node)
{
	node->visitAllChildren(this);
	out("halt\n");
}

void GeneratorVisitor::visit(BinaryExpressionNode *node)
{
	node->getLeft()->visit(this);
	node->getRight()->visit(this);

	switch (node->getOperator())
	{
	case '+':
		out("add");
		break;
	case '-':
		out("sub");
		break;
	case '*':
		out("mul");
		break;
	case '/':
		out("div");
		break;
	case '%':
		out("mod");
		break;
	}
	out("\n");
}

void GeneratorVisitor::visit(IntExpressionNode *node)
{
	out("push");
	out(node->getValue());
	out("\n");
}

void GeneratorVisitor::visit(PrintStatementNode *node)
{
	if (node->getExpression() == nullptr)
	{
		return;
	}

	node->getExpression()->visit(this);
	out("print\n");
}

void GeneratorVisitor::visit(UnaryExpressionNode *node)
{
	node->getExpr()->visit(this);

	switch (node->getOperator())
	{
	case '!':
		out("fact");
		break;
	}
	out("\n");
}

void GeneratorVisitor::visit(AsmStatementNode *node)
{
	std::string asm_ = node->getStringExpression()->getValue();
	out(asm_);
	out("\n");
}

void GeneratorVisitor::visitAllChildren(Node *node)
{
	node->visit(this);

	if (_file.is_open())
	{
		_file << _buffer.str();
		_file.close();
	}
}

std::string GeneratorVisitor::getOutput() const
{
	return _buffer.str();
}

void GeneratorVisitor::out(const std::string &text)
{
	_buffer << text;
}

void GeneratorVisitor::out(int64_t value)
{
	_buffer << " " << value;
}
