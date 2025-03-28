#include "GeneratorVisitor.h"
#include "Nodes.h"
#include "Error.h"

GeneratorVisitor::GeneratorVisitor(const std::string &output_filename):
    _outputFilename(output_filename)
{
}

GeneratorVisitor::~GeneratorVisitor()
{
}

void GeneratorVisitor::visitAllChildren(Node *node)
{
    node->visit(this);

    if (global::hadError)
    {
        return;
    }

    std::ofstream file(_outputFilename);

    if (file.is_open())
    {
        file << _buffer.str();
        file.close();

        printf("-> %s\n", _outputFilename.c_str());
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

    switch ((int)node->getOperator())
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
    case '<':
        out("lt");
        break;
    case '>':
        out("gt");
        break;
    case EQUAL:
        out("eq");
        break;
    case NOT_EQUAL:
        out("ne");
        break;
    case LESS_OR_EQUAL:
        out("le");
        break;
    case GREATER_OR_EQUAL:
        out("ge");
        break;
    case OR:
        out("or");
        break;
    case AND:
        out("and");
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
    case '-':
        out("neg");
        break;
    case INCREMENT:
        out("inc");
        break;
    case DECREMENT:
        out("dec");
        break;
    }
    out("\n");

    // if our expression was a variable, we need to store the result back into the variable
    if (node->getExpr()->isVariable())
    {
        auto var = dynamic_cast<VariableExpressionNode *>(node->getExpr());
        out("store");
        out(_variables[var->getSymbol()]);
        out("\n");
    }
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
        out("push");
        out(str[i]);
        out("\n");
    }

    out("push");
    out(str.size());
    out("\n");
}

void GeneratorVisitor::visit(ConcatNode *node)
{
    node->getLeft()->visit(this);
    node->getRight()->visit(this);

    out("concat\n");
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

void GeneratorVisitor::visit(VarDeclNode *node)
{
    auto symbol = node->getSymbol();
    auto expr = node->getExpression();

    if (expr->isNumeric())
    {
        _variables[symbol] = _variables.size();

        expr->visit(this);

        out("store");
        out(_variables[symbol]);
        out("\n");
    }
}

void GeneratorVisitor::visit(VariableExpressionNode *node)
{
    if (_variables.find(node->getSymbol()) == _variables.end())
    {
        error("variable " + node->getName() + " is not defined", node->getToken());
    }

    out("load");
    out(_variables[node->getSymbol()]);
    out("\n");
}

void GeneratorVisitor::visit(AssignNode *node)
{
    auto symbol = node->getSymbol();
    auto expr = node->getExpression();

    if (expr->isNumeric())
    {
        int id = _variables.size();
        if (_variables.find(symbol) != _variables.end())
        {
            id = _variables[symbol];
        }
        else
        {
            _variables[symbol] = id;
        }

        expr->visit(this);

        out("store");
        out(_variables[symbol]);
        out("\n");
    }
}

void GeneratorVisitor::visit(ForStatementNode *node)
{
    static int forCount = 1;
    int forId = forCount++;
    out("for" + std::to_string(forId) + "start:\n");
    node->getInit()->visit(this);
    out("for" + std::to_string(forId) + "loop:\n");
    node->getCondition()->visit(this);
    out("jz for" + std::to_string(forId) + "end\n");
    node->getStatement()->visit(this);
    node->getIncrement()->visit(this);
    out("jmp for" + std::to_string(forId) + "loop\n");
    out("for" + std::to_string(forId) + "end:\n");
}

void GeneratorVisitor::visit(WhileStatementNode *node)
{
    static int whileCount = 1;
    int whileId = whileCount++;
    out("while" + std::to_string(whileId) + "loop:\n");
    node->getCondition()->visit(this);
    out("jz while" + std::to_string(whileId) + "end\n");
    node->getBody()->visit(this);
    out("jmp while" + std::to_string(whileId) + "loop\n");
    out("while" + std::to_string(whileId) + "end:\n");
}

void GeneratorVisitor::visit(IfStatementNode *node)
{
    static int ifCount = 1;
    int ifId = ifCount++;
    node->getCondition()->visit(this);
    StatementNode *elseStatement = node->getElseStatement();

    if (elseStatement)
    {
        out("jz if" + std::to_string(ifId) + "else\n");
    }
    else
    {
        out("jz if" + std::to_string(ifId) + "end\n");
    }

    node->getStatement()->visit(this);

    out("jmp if" + std::to_string(ifId) + "end\n");
    
    if (elseStatement)
    {
        out("if" + std::to_string(ifId) + "else:\n");
        elseStatement->visit(this);
    }

    out("if" + std::to_string(ifId) + "end:\n");
}
