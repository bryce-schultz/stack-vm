#pragma once
#include <string>
#include "../ast/include/Nodes.h"

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

	// print_statement
	// asm_statement
	StatementNode *parseStatement();

	// print ( expression )
	PrintStatementNode *parsePrintStatement();

	// asm ( string_expression )
	AsmStatementNode *parseAsmStatement();

	// numeric_expression
	// string_expression
	ExpressionNode *parseExpression();

	// addit
	NumericExpressionNode *parseNumericExpression();

	// STRING string_expressionPP
	StringExpressionNode *parseStringExpression();

	// STRING
	// NUMBER
	StringExpressionNode *parseStringExpressionP();

	// + string_expressionP string_expressionPP
	// nothing
	StringExpressionNode *parseStringExpressionPP(StringExpressionNode *lhs);

	// term additPP
	NumericExpressionNode *parseAddit();
	// + term
	// - term
	NumericExpressionNode *parseAdditP(NumericExpressionNode *lhs);
	// additP additPP
	// nothing
	NumericExpressionNode *parseAdditPP(NumericExpressionNode *lhs);

	// exponent termPP
	NumericExpressionNode *parseTerm();
	// * exponent
	// / exponent
	// % exponent
	NumericExpressionNode *parseTermP(NumericExpressionNode *lhs);
	// termP termPP
	// nothing
	NumericExpressionNode *parseTermPP(NumericExpressionNode *lhs);

	// factorial exponentP
	NumericExpressionNode *parseExponent();
	// ^ factorial exponentP
	// nothing
	NumericExpressionNode *parseExponentP(NumericExpressionNode *);

	// primary factorialP
	NumericExpressionNode *parseFactorial();
	// ! factorialP
	// nothing
	NumericExpressionNode *parseFactorialP(NumericExpressionNode *lhs);

	// ( numeric_expression )
	// number
	NumericExpressionNode *parsePrimary();

private:
	std::string source;
	size_t index;
	int currentToken;
	std::string text;
	std::string oldText;

	int colno;
	int lineno;
	std::vector<std::string> errors;
	std::string filename;
};