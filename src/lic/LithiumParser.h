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
	Node *parseInternal(const std::string &source, const std::string &filename = "");

	Token peekToken();
	Token nextToken();

	// statements
	ProgramNode *parseProgram();

	// print_statement
	// asm_statement
	// expression
	// IDENTIFIER statementP
	// for
	StatementNode *parseStatement();

	// statements statement
	// statement
	StatementsNode *parseStatements();

	// { statements }
	BlockNode *parseBlock();

	// var_decl
	DeclNode *parseDeclaration();

	AssignNode *parseAssignment();

	// identifier = expression
	VarDeclNode *parseVarDeclaration();

	// print ( expression )
	PrintStatementNode *parsePrintStatement();

	// asm ( string_expression )
	AsmStatementNode *parseAsmStatement();

	// for ( var_decl ; expression ; statement ) block
	ForStatementNode *parseForStatement();

	// while ( expression ) block
	WhileStatementNode *parseWhileStatement();

	// for ( var_decl ; expression ; statement ) block
	// ForStatementNode *parseForStatement();

	// numeric_expression
	// string_expression
	ExpressionNode *parseExpression();

	// addit
	NumericExpressionNode *parseNumericExpression();

	// STRING string_expressionPP
	StringExpressionNode *parseStringExpression();

	// ;
	// ( expression_list ) ;
	// = expression ;
	StatementNode *parseStatementP(const Token &identifier);

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