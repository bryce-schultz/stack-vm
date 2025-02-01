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
	 Util.o

.PHONY: all clean depends

all: lic

Error.o: Error.cpp Error.h Location.h
GeneratorVisitor.o: GeneratorVisitor.cpp GeneratorVisitor.h IVisitor.h \
 Nodes.h Node.h ProgramNode.h StatementNode.h ExpressionNode.h \
 BinaryExpressionNode.h NumericExpressionNode.h OperatorNode.h \
 IntExpressionNode.h UnaryExpressionNode.h PrintStatementNode.h \
 AsmStatementNode.h StringExpressionNode.h ConcatNode.h
IVisitor.o: IVisitor.cpp IVisitor.h Nodes.h Node.h ProgramNode.h \
 StatementNode.h ExpressionNode.h BinaryExpressionNode.h \
 NumericExpressionNode.h OperatorNode.h IntExpressionNode.h \
 UnaryExpressionNode.h PrintStatementNode.h AsmStatementNode.h \
 StringExpressionNode.h ConcatNode.h
LithiumParser.o: LithiumParser.cpp LithiumParser.h Nodes.h Node.h \
 IVisitor.h ProgramNode.h StatementNode.h ExpressionNode.h \
 BinaryExpressionNode.h NumericExpressionNode.h OperatorNode.h \
 IntExpressionNode.h UnaryExpressionNode.h PrintStatementNode.h \
 AsmStatementNode.h StringExpressionNode.h ConcatNode.h Token.h \
 Location.h
Location.o: Location.cpp Location.h
Token.o: Token.cpp Token.h Location.h
TokenError.o: TokenError.cpp TokenError.h Error.h Location.h Token.h
Util.o: Util.cpp Util.h
main.o: main.cpp LithiumParser.h Nodes.h Node.h IVisitor.h ProgramNode.h \
 StatementNode.h ExpressionNode.h BinaryExpressionNode.h \
 NumericExpressionNode.h OperatorNode.h IntExpressionNode.h \
 UnaryExpressionNode.h PrintStatementNode.h AsmStatementNode.h \
 StringExpressionNode.h ConcatNode.h Token.h Location.h \
 GeneratorVisitor.h Util.h

.cpp.o:
	$(CC) $(CPPFLAGS) -c $<

lic: $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $(OBJS)

depends:
	$(CC) -MM *.cpp 1>&2

clean:
	rm -f $(OBJS) lic
