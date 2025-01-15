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
	out(".start\n");
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
	case '^':
		out("pow");
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
	auto expression = node->getExpression();

	if (!expression)
	{
		return;
	}

	expression->visit(this);

	if (expression->isString())
	{
		out("printstr");
	}
	else
	{
		out("print");
	}

	out("\n");
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

void GeneratorVisitor::visit(StringExpressionNode *node)
{
	std::string str = node->getValue();

	// loop backwards through the string
	for (int i = str.size() - 1; i >= 0; i--)
	{
		out("push ");
		out(str[i]);
		out("\n");
	}

	out("push ");
	out(str.size());
	out("\n");
}

void GeneratorVisitor::visit(ConcatNode *node)
{
	node->getLeft()->visit(this);
	node->getRight()->visit(this);

	out("concat\n");
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
