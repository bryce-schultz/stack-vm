cmake_minimum_required(VERSION 3.10)

# Compiler project
add_executable(compiler
    ast/src/UnaryExpressionNode.cpp
    ast/src/StatementNode.cpp
    ast/src/ProgramNode.cpp
    ast/src/PrintStatementNode.cpp
    ast/src/Node.cpp
    ast/src/OperatorNode.cpp
    ast/src/BinaryExpressionNode.cpp
    ast/src/IntExpressionNode.cpp
    src/main.cpp
    src/LithiumParser.cpp
    visitors/src/IVisitor.cpp
    visitors/generator/src/GeneratorVisitor.cpp
)

target_include_directories(compiler PRIVATE
    ast/include
    visitors/include
    visitors/generator/include
)