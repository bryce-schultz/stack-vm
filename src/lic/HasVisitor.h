//***********************************************
// HasVisitor.h
//***********************************************

#pragma once

#include <unordered_map>
#include <string>

#include "IVisitor.h"

// Special keys
// main - returns true if the start node contains the main function
// func_<name> - returns true if the start node contains a function with the given name
// return - returns true if the start node contains a return statement
// call - returns true if the start node contains a function call
// call_<name> - returns true if the start node contains a function call with the given name

class HasVisitor : public IVisitor
{
    public:
        virtual void visitAllChildren(Node *node) override;

        virtual void visit(FuncDeclNode *node) override;

        virtual void visit(ReturnStatementNode *node) override;

        virtual void visit(CallNode *node) override;

        bool has(const std::string &name) const;
    private:
        std::unordered_map<std::string, bool> hasMap;
};