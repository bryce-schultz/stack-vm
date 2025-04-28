#include "HasVisitor.h"
#include "Nodes.h"

void HasVisitor::visitAllChildren(Node *node)
{
    node->visit(this);
}

void HasVisitor::visit(FuncDeclNode *node)
{
    auto name = node->getSymbol()->getName();
    auto key = "func_" + name;
    hasMap[key] = true;

    // Check if the function is main.
    if (name == "main")
    {
        hasMap["main"] = true;
    }

    // Visit the parameter list and body of the function.
    auto paramList = node->getParamList();
    if (paramList)
    {
        paramList->visit(this);
    }
    auto body = node->getBody();
    if (body)
    {
        body->visit(this);
    }
}

void HasVisitor::visit(ReturnStatementNode *node)
{
    auto expr = node->getExpression();
    if (expr)
    {
        expr->visit(this);
    }
    // Mark that the function has a return statement.
    hasMap["return"] = true;
}

bool HasVisitor::has(const std::string &name) const
{
    auto it = hasMap.find(name);
    if (it != hasMap.end())
    {
        return it->second;
    }

    return false;
}
