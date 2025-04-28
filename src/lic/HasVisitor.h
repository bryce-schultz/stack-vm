#pragma once

#include <unordered_map>
#include <string>

#include "IVisitor.h"

class HasVisitor : public IVisitor
{
    public:
        virtual void visitAllChildren(Node *node) override;

        virtual void visit(FuncDeclNode *node) override;

        virtual void visit(ReturnStatementNode *node) override;

        bool has(const std::string &name) const;
    private:
        std::unordered_map<std::string, bool> hasMap;
};