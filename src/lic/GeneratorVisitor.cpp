//***********************************************
// GeneratorVisitor.cpp
//***********************************************

#include "GeneratorVisitor.h"
#include "Nodes.h"
#include "Error.h"

GeneratorVisitor::GeneratorVisitor(const std::string &output_filename):
    outputFilename(output_filename)
{
}

void GeneratorVisitor::visitAllChildren(Node *node)
{
    // Visit the root node.
    node->visit(this);

    // Don't attepmt to output to the file if there is an error.
    if (global::hadError)
    {
        return;
    }

    std::ofstream file(outputFilename);

    if (file.is_open())
    {
        file << outputBuffer.str();
        file.close();

        printf("-> %s\n", outputFilename.c_str());
    }
}

void GeneratorVisitor::visit(ProgramNode *node)
{
    // Visit the statements that are in the program.
    node->getStatements()->visit(this);
    // Tell the vm the program is done.
    out("halt\n");
}

void GeneratorVisitor::visit(BinaryExpressionNode *node)
{
    // Visit the left and right children of the binary expression.
    node->getLeft()->visit(this);
    node->getRight()->visit(this);

    // Get the operator of the binary expression and 
    // output the corresponding instruction.
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
    // Push the value of the integer expression onto the stack.
    out("push");
    out(node->getValue());
    out("\n");
}

void GeneratorVisitor::visit(PrintStatementNode *node)
{
    auto expression = node->getExpression();

    // Ensure the expression is not null.
    if (!expression)
    {
        return;
    }

    // Visit the expression.
    expression->visit(this);

    // Output the print instruction based on expression type.
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
    // Visit the expression of the unary expression.
    node->getExpr()->visit(this);

    // Get the operator of the unary expression and output the corresponding instruction.
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

    // If our expression was a variable, we store the result back into the variable.
    if (node->getExpr()->isVariable())
    {
        auto var = dynamic_cast<VariableExpressionNode *>(node->getExpr());
        out("store");
        out(variables[var->getSymbol()]);
        out("\n");
    }
}

void GeneratorVisitor::visit(AsmStatementNode *node)
{
    // Get the raw assembly code and output it.
    std::string asm_ = node->getStringExpression()->getValue();
    out(asm_);
    out("\n");
}

void GeneratorVisitor::visit(StringExpressionNode *node)
{
    // Get the string value of the string expression.
    std::string str = node->getValue();

    // Loop backwards through the string, pushing each character onto the stack.
    for (int i = str.size() - 1; i >= 0; i--)
    {
        out("push");
        out(str[i]);
        out("\n");
    }

    // Push the size of the string onto the stack.
    out("push");
    out(str.size());
    out("\n");
}

void GeneratorVisitor::visit(ConcatNode *node)
{
    // Visit the left and right children of the concat node.
    node->getLeft()->visit(this);
    node->getRight()->visit(this);

    // Output the concat instruction.
    out("concat\n");
}

std::string GeneratorVisitor::getOutput() const
{
    return outputBuffer.str();
}

void GeneratorVisitor::out(const std::string &text)
{
    outputBuffer << text;
}

void GeneratorVisitor::out(int64_t value)
{
    outputBuffer << " " << value;
}

void GeneratorVisitor::visit(VarDeclNode *node)
{
    // Get the symbol and expression of the variable declaration.
    auto symbol = node->getSymbol();
    auto expr = node->getExpression();

    // Ensure the expression is numeric as string variables are not supported yet.
    if (expr->isNumeric())
    {
        // Store the variable in the variables map if it doesn't already exist.
        // We use the symbol as the key so that variables in different scopes can have the same name.
        if (variables.find(symbol) == variables.end())
        {
            variables[symbol] = variables.size();
        }

        // Visit the expression and store the result in the variable.
        expr->visit(this);

        out("store");
        out(variables[symbol]);
        out("\n");
    }
}

void GeneratorVisitor::visit(VariableExpressionNode *node)
{
    // Get the symbol and name of the variable expression.
    auto symbol = node->getSymbol();
    auto name = node->getName();
    auto token = node->getToken();

    // Ensure the variable is defined.
    if (variables.find(symbol) == variables.end())
    {
        undefined(token);
    }

    // Load the variable onto the stack.
    out("load");
    out(variables[symbol]);
    out("\n");
}

void GeneratorVisitor::visit(AssignNode *node)
{
    // Get the symbol and expression of the assignment.
    auto symbol = node->getSymbol();
    auto expr = node->getExpression();

    if (expr->isNumeric())
    {
        // Store the variable in the variables map if it doesn't already exist.
        if (variables.find(symbol) == variables.end())
        {
            variables[symbol] = variables.size();
        }

        // Visit the expression and store the result in the variable.
        expr->visit(this);

        out("store");
        out(variables[symbol]);
        out("\n");
    }
}

void GeneratorVisitor::visit(ForStatementNode *node)
{   
    // This is a static variable so this is only set once.
    static int forCount = 1;
    // Create a unique id for the for loop.
    int forId = forCount++;
    
    // Create a starting label for the for loop and visit the init expression.
    out("for" + std::to_string(forId) + "start:\n");
    node->getInit()->visit(this);

    // Create a label for the loop and visit the condition.
    // We always want to jump back to before the condition check.
    out("for" + std::to_string(forId) + "loop:\n");
    node->getCondition()->visit(this);

    // If the condition evaluates to 0 (false), jump to the end of the for loop.
    out("jz for" + std::to_string(forId) + "end\n");

    // Visit the statement and increment expression.
    node->getStatement()->visit(this);
    node->getIncrement()->visit(this);

    // Jump back to the start of the for loop.
    out("jmp for" + std::to_string(forId) + "loop\n");

    // Create a label for the end of the for loop.
    out("for" + std::to_string(forId) + "end:\n");
}

void GeneratorVisitor::visit(WhileStatementNode *node)
{
    // This is a static variable so this is only set once.
    static int whileCount = 1;
    // Create a unique id for the while loop.
    int whileId = whileCount++;

    // Create a label for the start of the while loop and visit the condition.
    out("while" + std::to_string(whileId) + "loop:\n");
    node->getCondition()->visit(this);

    // If the condition evaluates to 0 (false), jump to the end of the while loop.
    out("jz while" + std::to_string(whileId) + "end\n");

    // Visit the body of the while loop if it exists.
    if (node->getBody())
    {
        node->getBody()->visit(this);
    }

    // Jump back to the start of the while loop.
    out("jmp while" + std::to_string(whileId) + "loop\n");

    // Create a label for the end of the while loop.
    out("while" + std::to_string(whileId) + "end:\n");
}

void GeneratorVisitor::visit(IfStatementNode *node)
{
    // This is a static variable so this is only set once.
    static int ifCount = 1;
    // Create a unique id for the if statement.
    int ifId = ifCount++;

    // Visit the condition of the if statement.
    node->getCondition()->visit(this);

    // If the if statement has an else statement, jump to the else statement if the condition is false.
    // Otherwise, jump to the end of the if statement.
    StatementNode *elseStatement = node->getElseStatement();
    if (elseStatement)
    {
        out("jz if" + std::to_string(ifId) + "else\n");
    }
    else
    {
        out("jz if" + std::to_string(ifId) + "end\n");
    }

    // Visit the body of the if statement.
    node->getStatement()->visit(this);

    // Jump to the end of the if statement.
    out("jmp if" + std::to_string(ifId) + "end\n");
    
    // If the if statement has an else statement, visit the else statement.
    if (elseStatement)
    {
        // Create a label for the else statement and visit it.
        out("if" + std::to_string(ifId) + "else:\n");
        elseStatement->visit(this);
    }

    // Create a label for the end of the if statement.
    out("if" + std::to_string(ifId) + "end:\n");
}
