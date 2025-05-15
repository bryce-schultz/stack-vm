#include "PrintVisitor.h"
#include "Nodes.h"

PrintVisitor::PrintVisitor():
    indentLevel(0)
{
}

void PrintVisitor::visitAllChildren(Node* node)
{
    // Visit the root node.
    node->visit(this);
}

void PrintVisitor::visit(ProgramNode* node)
{
	openTag("Program");
    node->getStatements()->visit(this);
    closeTag("Program");
}

void PrintVisitor::visit(StatementNode* node)
{
    openTag("Statement");
    node->visitAllChildren(this);
    closeTag("Statement");
}

void PrintVisitor::visit(ExpressionNode* node)
{
    openTag("Expression");
    node->visitAllChildren(this);
    closeTag("Expression");
}

void PrintVisitor::visit(BinaryExpressionNode* node)
{
    openTag("BinaryExpression");
    node->visitAllChildren(this);
    closeTag("BinaryExpression");
}

void PrintVisitor::visit(OperatorNode* node)
{
    openTag("Operator", {{"value", tokenTypeToString(node->getValue())}}, true);
    node->visitAllChildren(this);
}

void PrintVisitor::visit(UnaryExpressionNode* node)
{
    openTag("UnaryExpression");
    node->visitAllChildren(this);
    closeTag("UnaryExpression");
}

void PrintVisitor::visit(PrintStatementNode* node)
{
    openTag("PrintStatement");
    node->visitAllChildren(this);
    closeTag("PrintStatement");
}

void PrintVisitor::visit(AsmStatementNode* node)
{
    openTag("AsmStatement");
    node->visitAllChildren(this);
    closeTag("AsmStatement");
}

void PrintVisitor::visit(NumericExpressionNode* node)
{
    openTag("NumericExpression");
    node->visitAllChildren(this);
    closeTag("NumericExpression");
}

void PrintVisitor::visit(StringExpressionNode* node)
{
    openTag("StringExpression");
    node->visitAllChildren(this);
    closeTag("StringExpression");
}

void PrintVisitor::visit(NumberExpressionNode* node)
{
    std::string value = node->isInteger() ? std::to_string(node->getValueAsInt()) : std::to_string(node->getValue());
    openTag("NumberExpression", {{"value", value}}, true);
    node->visitAllChildren(this);
}

void PrintVisitor::visit(ConcatNode* node)
{
    openTag("Concat");
    node->visitAllChildren(this);
    closeTag("Concat");
}

void PrintVisitor::visit(DeclNode* node)
{
    openTag("Decl");
    node->visitAllChildren(this);
    closeTag("Decl");
}

void PrintVisitor::visit(VarDeclNode* node)
{
    openTag("VarDecl", { {"name", node->getName()} });
    node->visitAllChildren(this);
    closeTag("VarDecl");
}

void PrintVisitor::visit(VariableExpressionNode* node)
{
    openTag("VariableExpression", {{"name", node->getName()}}, true);
}

void PrintVisitor::visit(AssignNode* node)
{
    openTag("Assign");
    node->visitAllChildren(this);
    closeTag("Assign");
}

void PrintVisitor::visit(StatementsNode* node)
{
    openTag("Statements");
    node->visitAllChildren(this);
    closeTag("Statements");
}

void PrintVisitor::visit(ForStatementNode* node)
{
    openTag("ForStatement");
    node->visitAllChildren(this);
    closeTag("ForStatement");
}

void PrintVisitor::visit(WhileStatementNode* node)
{
    openTag("WhileStatement");
    node->visitAllChildren(this);
    closeTag("WhileStatement");
}

void PrintVisitor::visit(IfStatementNode* node)
{
    openTag("IfStatement");
    node->visitAllChildren(this);
    closeTag("IfStatement");
}

void PrintVisitor::visit(BlockNode* node)
{
    openTag("Block");
    node->visitAllChildren(this);
    closeTag("Block");
}

void PrintVisitor::visit(StatementListNode* node)
{
    openTag("StatementList");
    node->visitAllChildren(this);
    closeTag("StatementList");
}

void PrintVisitor::visit(FuncDeclNode* node)
{
    openTag("FuncDecl", {{"name", node->getName()}});
    node->visitAllChildren(this);
    closeTag("FuncDecl");
}

void PrintVisitor::visit(ParamNode* node)
{
    openTag("Param", {{"name", node->getName()}}, true);
}

void PrintVisitor::visit(ParamListNode* node)
{
    openTag("ParamList");
    node->visitAllChildren(this);
    closeTag("ParamList");
}

void PrintVisitor::visit(ArgNode* node)
{
    openTag("Arg");
   node->visitAllChildren(this);
    closeTag("Arg");
}

void PrintVisitor::visit(ArgListNode* node)
{
    openTag("ArgList");
    node->visitAllChildren(this);
    closeTag("ArgList");
}

void PrintVisitor::visit(CallNode* node)
{
    openTag("Call", {{ "name", node->getName() }});
    node->visitAllChildren(this);
    closeTag("Call");
}

void PrintVisitor::visit(Symbol* node)
{
    openTag("Symbol");
    node->visitAllChildren(this);
    closeTag("Symbol");
}

void PrintVisitor::visit(ReturnStatementNode* node)
{
    openTag("ReturnStatement");
    node->visitAllChildren(this);
    closeTag("ReturnStatement");
}

void PrintVisitor::visit(StringConversionNode* node)
{
    openTag("StringConversion");
    node->visitAllChildren(this);
    closeTag("StringConversion");
}


void PrintVisitor::indent()
{
    indentLevel++;
}

void PrintVisitor::deindent()
{
    if (indentLevel > 0)
    {
        indentLevel--;
    }
}

void PrintVisitor::openTag(const std::string &tag, const std::map<std::string, std::string> &attributes, bool closeTag)
{
    for (int i = 0; i < indentLevel; i++)
    {
        outputBuffer << "  ";
    }

    outputBuffer << "<" << tag;

    for (const auto &attribute : attributes)
    {
        outputBuffer << " " << attribute.first << "=\"" << attribute.second << "\"";
    }

    if (closeTag)
    {
        outputBuffer << "/>\n";
    }
    else
    {
        outputBuffer << ">\n";
        indent();
    }
}

void PrintVisitor::closeTag(const std::string &tag)
{
    deindent();
    for (int i = 0; i < indentLevel; i++)
    {
        outputBuffer << "  ";
    }

    outputBuffer << "</" << tag << ">\n";
}

std::string PrintVisitor::getOutput() const
{
    return outputBuffer.str();
}

void PrintVisitor::outputToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (file.is_open())
    {
        file << outputBuffer.str();
        file.close();
        std::cout << "-> " << filename << std::endl;
    }
    else
    {
        general_error("Failed to open file for writing: " + filename);
    }
}
