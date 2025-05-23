//***********************************************
// GeneratorVisitor.h
//***********************************************

#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <cstdint>

#include "IVisitor.h"
#include "Symbol.h"

//***********************************************
// GeneratorVisitor
//
// This class is responsible for generating the
// output code. It visits the AST and generates
// the lsm code.
//
// If provided, the outputFilename is used to
// write the generated code to a file.
// Otherwise, the output is written to an
// internal buffer, and the getOutput() method
// can be used to retrieve the generated code.
//***********************************************
class GeneratorVisitor : public IVisitor
{
public:
    //***********************************************
    // Constructor
    //
    // outputFilename: The name of the file to write
    // the generated code to.
    // If empty, the output is written to an
    // internal buffer. Use getOutput() to retrieve
    // the generated code.
    //***********************************************
    GeneratorVisitor(const std::string &outputFilename = "");

    //***********************************************
    // visitAllChildren
    //
    // Visits all children of the given node.
    // Used as the entry point for visiting the AST.
    //***********************************************
    virtual void visitAllChildren(Node *node) override;

    //***********************************************
    // visit (ProgramNode) - not to be called outside
    // of the visitor.
    //
    // Visits the ProgramNode.
    //***********************************************
    virtual void visit(ProgramNode *node) override;

    //***********************************************
    // visit (BinaryExpressionNode) - not to be called
    // outside of the visitor.
    //
    // Visits the BinaryExpressionNode.
    //***********************************************
    virtual void visit(BinaryExpressionNode *node) override;

    //***********************************************
    // visit (NumberExpressionNode) - not to be called outside
    // of the visitor.
    //
    // Visits the NumberExpressionNode.
    //***********************************************
    virtual void visit(NumberExpressionNode *node) override;

    //***********************************************
    // visit (PrintStatementNode) - not to be called
    // outside of the visitor.
    //
    // Visits the PrintStatementNode.
    //***********************************************
    virtual void visit(PrintStatementNode *node) override;

    //***********************************************
    // visit (UnaryExpressionNode) - not to be called
    // outside of the visitor.
    //
    // Visits the UnaryExpressionNode.
    //***********************************************
    virtual void visit(UnaryExpressionNode *node) override;

    //***********************************************
    // visit (AsmStatementNode) - not to be called
    // outside of the visitor.
    //
    // Visits the AsmStatementNode.
    //***********************************************
    virtual void visit(AsmStatementNode *node) override;

    //***********************************************
    // visit (ExpressionNode) - not to be called
    // outside of the visitor.
    //
    // Visits the ExpressionNode.
    //***********************************************
    virtual void visit(StringExpressionNode *node) override;

    //***********************************************
    // visit (DeclNode) - not to be called outside of
    // the visitor.
    //
    // Visits the DeclNode.
    //***********************************************
    virtual void visit(VarDeclNode *node) override;

    //***********************************************
    // visit (VariableExpressionNode) - not to be called
    // outside of the visitor.
    //
    // Visits the VariableExpressionNode.
    //***********************************************
    virtual void visit(VariableExpressionNode *node) override;

    //***********************************************
    // visit (AssignNode) - not to be called outside of
    // the visitor.
    //
    // Visits the AssignNode.
    //***********************************************
    virtual void visit(AssignNode *node) override;

    //***********************************************
    // visit (StatementsNode) - not to be called outside
    // of the visitor.
    //
    // Visits the StatementsNode.
    //***********************************************
    virtual void visit(ForStatementNode *node) override;

    //***********************************************
    // visit (WhileStatementNode) - not to be called
    // outside of the visitor.
    //
    // Visits the WhileStatementNode.
    //***********************************************
    virtual void visit(WhileStatementNode *node) override;

    //***********************************************
    // visit (IfStatementNode) - not to be called outside
    // of the visitor.
    //
    // Visits the IfStatementNode.
    //***********************************************
    virtual void visit(IfStatementNode *node) override;

    //***********************************************
    // visit (FuncDeclNode) - not to be called outside
    // of the visitor.
    //
    // Visits the FuncDeclNode.
    //***********************************************
    virtual void visit(FuncDeclNode *node) override;

    //***********************************************
    // visit (CallNode) - not to be called outside
    // of the visitor.
    //
    // Visits the CallNode.
    //***********************************************
    virtual void visit(CallNode *node) override;

    //***********************************************
    // visit (ParamListNode) - not to be called outside
    // of the visitor.
    //
    // Visits the ParamListNode.
    //***********************************************
    virtual void visit(ParamListNode *node) override;

    //***********************************************
    // visit (ParamNode) - not to be called outside
    // of the visitor.
    //
    // Visits the ParamNode.
    //***********************************************
    virtual void visit(ParamNode *node) override;

    //***********************************************
    // visit (ReturnStatementNode) - not to be called
    // outside of the visitor.
    //
    // Visits the ReturnStatementNode.
    //***********************************************
    virtual void visit(ReturnStatementNode *node) override;

    //***********************************************
    // visit (StringConversionNode) - not to be called outside
    // of the visitor.
    //
    // Visits the StringConversionNode.
    //***********************************************
    virtual void visit(StringConversionNode *node) override;
    
    //***********************************************
    // getOutput
    // 
    // Returns the generated code.
    //***********************************************
    std::string getOutput() const;
private:
    //***********************************************
    // out (string)
    //
    // Writes the given text to the output.
    //***********************************************
    void out(const std::string &text);

    //***********************************************
    // out (int)
    //
    // Writes the given value to the output.
    //***********************************************
    void out(int64_t value);
private:
    std::string outputFilename;
    std::stringstream outputBuffer;
    std::unordered_map<Symbol *, uint64_t> variables;
};