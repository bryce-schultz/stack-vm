#pragma once

#include <string>

#include "Nodes.h"
#include "Token.h"
#include "LithiumSymbolTable.h"
#include "LithiumTokenizer.h"

class LithiumParser
{
public:
	LithiumParser();

	Node *parse(const std::string &source);
	std::vector<std::string> getErrors() const;
private:
	void _error(const std::string &message, const Token &token, const char* file = nullptr, size_t lineno = 0);
	Node *parseInternal(const std::string &source, const std::string &filename = "");

	Token peekToken();
	Token nextToken();

	// statements statement
	// statement
	ProgramNode *parseProgram();

	// print_statement
	// asm_statement
	// expression
	// declaration
	// for
	StatementNode *parseStatement();

	// var_decl
	DeclNode *parseDeclaration();

	// identifier = expression
	VarDeclNode *parseVarDeclaration();

	// print ( expression )
	PrintStatementNode *parsePrintStatement();

	// asm ( string_expression )
	AsmStatementNode *parseAsmStatement();

	// for ( var_decl ; expression ; statement ) statement
	// ForStatementNode *parseForStatement();

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

	// fact exponentP
	NumericExpressionNode *parseExponent();
	// ^ fact exponentP
	// nothing
	NumericExpressionNode *parseExponentP(NumericExpressionNode *);

	// - factorial
	// factorial
	NumericExpressionNode *parseFact();

	// primary factorialP
	NumericExpressionNode *parseFactorial();
	// ! factorialP
	// nothing
	NumericExpressionNode *parseFactorialP(NumericExpressionNode *lhs);

	// ( numeric_expression )
	// number
	NumericExpressionNode *parsePrimary();
private:
	LithiumTokenizer tokenizer;
	std::vector<std::string> errors;
};