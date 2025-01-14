#pragma once
#include <string>
#include "Nodes.h"

enum TokenType
{
	NONE = 1000,
	NUMBER,
	PRINT,
	IDENTIFIER,
	STRING,
	ASM,
	END,
	JUNK
};

class LithiumParser
{
public:
	LithiumParser();
	~LithiumParser();

	Node *parse(const std::string &source);

	std::vector<std::string> getErrors() const;

private:
	void error(const std::string &message);
	Node *parseInternal(const std::string &source);

	int peekToken();
	int getToken();
	int nextToken();

	bool isWhitespace(char c) const;

	// statements statement
	// statement
	ProgramNode *parseProgram();

	// expression
	StatementNode *parseStatement();

	// print statement
	PrintStatementNode *parsePrintStatement();

	// asm statement
	AsmStatementNode *parseAsmStatement();

	// addit
	ExpressionNode *parseExpression();

	// term additPP
	ExpressionNode *parseAddit();
	// + term
	// - term
	ExpressionNode *parseAdditP(ExpressionNode *lhs);
	// additP additPP
	// nothing
	ExpressionNode *parseAdditPP(ExpressionNode *lhs);

	// exponent termPP
	ExpressionNode *parseTerm();
	// * exponent
	// / exponent
	// % exponent
	ExpressionNode *parseTermP(ExpressionNode *lhs);
	// termP termPP
	// nothing
	ExpressionNode *parseTermPP(ExpressionNode *lhs);

	// factorial exponentP
	ExpressionNode *parseExponent();
	// ^ factorial exponentP
	// nothing
	ExpressionNode *parseExponentP();

	// primary factorialP
	ExpressionNode *parseFactorial();
	// ! factorialP
	// nothing
	ExpressionNode *parseFactorialP(ExpressionNode *lhs);

	// ( expression )
	// number
	ExpressionNode *parsePrimary();

private:
	std::string source;
	size_t index;
	int currentToken;
	std::string text;

	int colno;
	int lineno;
	std::vector<std::string> errors;
	std::string filename;
};