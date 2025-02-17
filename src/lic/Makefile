CC=g++
CPPFLAGS=-g -Wall -Wextra -pedantic -std=c++11

OBJS=main.o \
     LithiumParser.o \
	 Token.o \
	 TokenError.o \
	 Location.o \
	 Error.o \
	 IVisitor.o \
	 GeneratorVisitor.o \
	 Util.o \
	 LithiumCompiler.o \
	 LithiumSymbolTable.o \
	 SymbolTable.o \
	 LithiumTokenizer.o

.PHONY: all clean depends

all: lic tokenizer_tester

Error.o: Error.cpp Error.h Location.h
GeneratorVisitor.o: GeneratorVisitor.cpp GeneratorVisitor.h IVisitor.h \
 Nodes.h Node.h ProgramNode.h StatementNode.h ExpressionNode.h \
 BinaryExpressionNode.h NumericExpressionNode.h OperatorNode.h \
 IntExpressionNode.h UnaryExpressionNode.h PrintStatementNode.h \
 AsmStatementNode.h StringExpressionNode.h ConcatNode.h DeclNode.h \
 VarDeclNode.h Symbol.h Token.h Location.h
IVisitor.o: IVisitor.cpp IVisitor.h Nodes.h Node.h ProgramNode.h \
 StatementNode.h ExpressionNode.h BinaryExpressionNode.h \
 NumericExpressionNode.h OperatorNode.h IntExpressionNode.h \
 UnaryExpressionNode.h PrintStatementNode.h AsmStatementNode.h \
 StringExpressionNode.h ConcatNode.h DeclNode.h VarDeclNode.h Symbol.h \
 Token.h Location.h
LithiumCompiler.o: LithiumCompiler.cpp LithiumCompiler.h LithiumParser.h \
 Nodes.h Node.h IVisitor.h ProgramNode.h StatementNode.h ExpressionNode.h \
 BinaryExpressionNode.h NumericExpressionNode.h OperatorNode.h \
 IntExpressionNode.h UnaryExpressionNode.h PrintStatementNode.h \
 AsmStatementNode.h StringExpressionNode.h ConcatNode.h DeclNode.h \
 VarDeclNode.h Symbol.h Token.h Location.h LithiumSymbolTable.h \
 SymbolTable.h LithiumTokenizer.h GeneratorVisitor.h Util.h
LithiumParser.o: LithiumParser.cpp LithiumParser.h Nodes.h Node.h \
 IVisitor.h ProgramNode.h StatementNode.h ExpressionNode.h \
 BinaryExpressionNode.h NumericExpressionNode.h OperatorNode.h \
 IntExpressionNode.h UnaryExpressionNode.h PrintStatementNode.h \
 AsmStatementNode.h StringExpressionNode.h ConcatNode.h DeclNode.h \
 VarDeclNode.h Symbol.h Token.h Location.h LithiumSymbolTable.h \
 SymbolTable.h LithiumTokenizer.h
LithiumSymbolTable.o: LithiumSymbolTable.cpp LithiumSymbolTable.h \
 SymbolTable.h Symbol.h Token.h Location.h DeclNode.h StatementNode.h \
 Node.h IVisitor.h
LithiumTokenizer.o: LithiumTokenizer.cpp LithiumTokenizer.h Token.h \
 Location.h
Location.o: Location.cpp Location.h
SymbolTable.o: SymbolTable.cpp SymbolTable.h Symbol.h Token.h Location.h \
 DeclNode.h StatementNode.h Node.h IVisitor.h
Token.o: Token.cpp Token.h Location.h
TokenError.o: TokenError.cpp TokenError.h Error.h Location.h Token.h
Util.o: Util.cpp Util.h
main.o: main.cpp LithiumCompiler.h LithiumParser.h Nodes.h Node.h \
 IVisitor.h ProgramNode.h StatementNode.h ExpressionNode.h \
 BinaryExpressionNode.h NumericExpressionNode.h OperatorNode.h \
 IntExpressionNode.h UnaryExpressionNode.h PrintStatementNode.h \
 AsmStatementNode.h StringExpressionNode.h ConcatNode.h DeclNode.h \
 VarDeclNode.h Symbol.h Token.h Location.h LithiumSymbolTable.h \
 SymbolTable.h LithiumTokenizer.h GeneratorVisitor.h
tokenizer_tester.o: tokenizer_tester.cpp LithiumTokenizer.h Token.h \
 Location.h Util.h

.cpp.o:
	$(CC) $(CPPFLAGS) -c $<

lic: $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $(OBJS)

tokenizer_tester: tokenizer_tester.o LithiumTokenizer.o Token.o Location.o Util.o
	$(CC) $(CPPFLAGS) -o $@ tokenizer_tester.o LithiumTokenizer.o Token.o Location.o Util.o

depends:
	$(CC) -MM *.cpp 1>&2

clean:
	rm -f $(OBJS) lic
