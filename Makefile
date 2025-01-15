MAKEFLAGS := --jobs=$(shell nproc)
CC=g++
CPPFLAGS=-g -Wall -Werror -Wextra -pedantic -Wno-literal-suffix
OBJS=main.o \
 GeneratorVisitor.o \
 LithiumParser.o \
 SVASMParser.o \
 ProgramNode.o \
 StatementNode.o \
 BinaryExpressionNode.o \
 OperatorNode.o \
 IntExpressionNode.o \
 UnaryExpressionNode.o \
 PrintStatementNode.o \
 Node.o \
 IVisitor.o \
 SimpleVirtualMachine.o \
 SVMException.o

all: svm

clean:
	rm -f $(OBJS) svm
	rm -f tests/*.svasm

.cpp.o:
	$(CC) $(CPPFLAGS) -c $<

BinaryExpressionNode.o: BinaryExpressionNode.cpp BinaryExpressionNode.h \
 NumericExpressionNode.h ExpressionNode.h Node.h IVisitor.h \
 OperatorNode.h
GeneratorVisitor.o: GeneratorVisitor.cpp GeneratorVisitor.h IVisitor.h \
 Nodes.h Node.h ProgramNode.h StatementNode.h ExpressionNode.h \
 BinaryExpressionNode.h NumericExpressionNode.h OperatorNode.h \
 IntExpressionNode.h UnaryExpressionNode.h PrintStatementNode.h \
 AsmStatementNode.h StringExpressionNode.h
IVisitor.o: IVisitor.cpp IVisitor.h Nodes.h Node.h ProgramNode.h \
 StatementNode.h ExpressionNode.h BinaryExpressionNode.h \
 NumericExpressionNode.h OperatorNode.h IntExpressionNode.h \
 UnaryExpressionNode.h PrintStatementNode.h AsmStatementNode.h \
 StringExpressionNode.h
IntExpressionNode.o: IntExpressionNode.cpp IntExpressionNode.h \
 NumericExpressionNode.h ExpressionNode.h Node.h IVisitor.h
LithiumParser.o: LithiumParser.cpp LithiumParser.h Nodes.h Node.h \
 IVisitor.h ProgramNode.h StatementNode.h ExpressionNode.h \
 BinaryExpressionNode.h NumericExpressionNode.h OperatorNode.h \
 IntExpressionNode.h UnaryExpressionNode.h PrintStatementNode.h \
 AsmStatementNode.h StringExpressionNode.h
Node.o: Node.cpp Node.h IVisitor.h
OperatorNode.o: OperatorNode.cpp OperatorNode.h IVisitor.h Node.h
PrintStatementNode.o: PrintStatementNode.cpp PrintStatementNode.h \
 StatementNode.h Node.h IVisitor.h ExpressionNode.h
ProgramNode.o: ProgramNode.cpp ProgramNode.h Node.h IVisitor.h
SVASMParser.o: SVASMParser.cpp SVASMParser.h Program.h \
 SimpleVirtualMachine.h
SVMException.o: SVMException.cpp SVMException.h
SimpleVirtualMachine.o: SimpleVirtualMachine.cpp SVMException.h \
 SimpleVirtualMachine.h
StatementNode.o: StatementNode.cpp StatementNode.h Node.h IVisitor.h
UnaryExpressionNode.o: UnaryExpressionNode.cpp UnaryExpressionNode.h \
 NumericExpressionNode.h ExpressionNode.h Node.h IVisitor.h \
 OperatorNode.h
main.o: main.cpp SimpleVirtualMachine.h SVMException.h SVASMParser.h \
 Program.h LithiumParser.h Nodes.h Node.h IVisitor.h ProgramNode.h \
 StatementNode.h ExpressionNode.h BinaryExpressionNode.h \
 NumericExpressionNode.h OperatorNode.h IntExpressionNode.h \
 UnaryExpressionNode.h PrintStatementNode.h AsmStatementNode.h \
 StringExpressionNode.h GeneratorVisitor.h

svm: $(OBJS)
	$(CC) $(CPPFLAGS) -o svm $(OBJS)
