//***********************************************
// LithiumParser.h
//***********************************************

#pragma once

#include <string>

#include "Nodes.h"
#include "Token.h"
#include "LithiumSymbolTable.h"
#include "LithiumTokenizer.h"

template <typename T>
class ParseResult
{
public:
	ParseResult(bool success, T* node = nullptr):
		success(success),
		node(node)
	{
	}

	bool isValid() const
	{
		return success;
	}

	T* getNode() const
	{
		return node;
	}
private:
	bool success;
	T* node;
};

class LithiumParser
{
public:
	LithiumParser();

	ParseResult<Node> parse(const std::string &source);
	std::vector<std::string> getErrors() const;
private:
	ParseResult<Node> parseInternal(const std::string &source, const std::string &filename = "");

	Token peekToken();
	Token nextToken();

	// program -> statements
	ParseResult<ProgramNode> parseProgram();

	// statements -> statement statements
	//             | nothing
	ParseResult<StatementsNode> parseStatements();

	// statement -> single_statement ;
	//            | block
	//            | for_statement
	//            | while_statement
	//            | if_statement
	ParseResult<StatementNode> parseStatement();
	
	// statement_list -> single_statement statement_list'
	ParseResult<StatementListNode> parseStatementList();

	// statement_list' -> , single_statement statement_list'
	//                  | nothing
	ParseResult<StatementListNode> parseStatementListP(StatementNode *lhs);

	// single_statement -> expression
	//                   | print_statement
	//                   | asm_statement
	ParseResult<StatementNode> parseSingleStatement();

	// block -> { statements }
	ParseResult<BlockNode> parseBlock();

	// if_statement -> IF ( numeric_expression ) statement if_statement'
	ParseResult<IfStatementNode> parseIfStatement();

	// if_statement' -> ELSE statement
	//                | nothing
	ParseResult<StatementNode> parseIfStatementP();

	// while_statement -> WHILE ( numeric_expression ) statement
	ParseResult<WhileStatementNode> parseWhileStatement();

	// for -> FOR ( statement_list ; numeric_expression ; statement_list ) statement
	ParseResult<ForStatementNode> parseForStatement();

	// asm_statement -> ASM ( string_expression )
	ParseResult<AsmStatementNode> parseAsmStatement();

	// print_statement -> PRINT ( expression )
	ParseResult<PrintStatementNode> parsePrintStatement();

	// expression -> numeric_expression
	// 			   | string_expression
	ParseResult<ExpressionNode> parseExpression();

	// string_expression -> STRING string_expression'
	ParseResult<StringExpressionNode> parseStringExpression();

	// string_expression' -> + string_expression
	//                     | nothing
	ParseResult<StringExpressionNode> parseStringExpressionP(StringExpressionNode *lhs);

	// numeric_expression -> assignment
	ParseResult<NumericExpressionNode> parseNumericExpression();

	// assignment -> optional assignment'
	ParseResult<NumericExpressionNode> parseAssignment();

	// assignment' -> = expression
	//              | nothing
	ParseResult<NumericExpressionNode> parseAssignmentP(VariableExpressionNode *lhs);

	// optional -> compound optional'
	ParseResult<NumericExpressionNode> parseOptional();

	// optional' -> || compound optional'
	// 			  | nothing
	ParseResult<NumericExpressionNode> parseOptionalP(NumericExpressionNode *lhs);

	// compound -> equality compound'
	ParseResult<NumericExpressionNode> parseCompound();

	// compound' -> && equality compound'
	//			  | nothing
	ParseResult<NumericExpressionNode> parseCompoundP(NumericExpressionNode *lhs);

	// equality -> comparison equality'
	ParseResult<NumericExpressionNode> parseEquality();

	// equality' -> == comparison equality'
	//            | != comparison equality'
	//            | nothing
	ParseResult<NumericExpressionNode> parseEqualityP(NumericExpressionNode *lhs);

	// comparison -> addit comparison'
	ParseResult<NumericExpressionNode> parseComparison();

	// comparison' -> < addit comparison'
	// 				| > addit comparison'
	// 				| <= addit comparison'
	// 				| >= addit comparison'
	ParseResult<NumericExpressionNode> parseComparisonP(NumericExpressionNode *lhs);

	// addit -> term addit'
	ParseResult<NumericExpressionNode> parseAddit();

	// addit' -> + term addit'
	//         | - term addit'
    //         | nothing
	ParseResult<NumericExpressionNode> parseAdditP(NumericExpressionNode *lhs);


	// term -> exponent term'
	ParseResult<NumericExpressionNode> parseTerm();

	// term' -> * exponent
	//        | / exponent
	//        | % exponent
	//        | nothing
	ParseResult<NumericExpressionNode> parseTermP(NumericExpressionNode *lhs);

	// exponent -> fact exponent'
	ParseResult<NumericExpressionNode>  parseExponent();

	// exponent' -> ^ fact exponent'
	// 			  | nothing
	ParseResult<NumericExpressionNode> parseExponentP(NumericExpressionNode *);

	// fact -> - factorial
	//       | factorial
	ParseResult<NumericExpressionNode> parseFact();

	// factorial -> primary factorial'
	ParseResult<NumericExpressionNode> parseFactorial();

	// factorial' -> ! factorial'
	//             | nothing
	ParseResult<NumericExpressionNode> parseFactorialP(NumericExpressionNode *lhs);

	// modifier -> primary modifier'
	ParseResult<NumericExpressionNode> parseModifier();

	// modifier' -> ++ modifier'
	//            | -- modifier'
	//            | nothing
	ParseResult<NumericExpressionNode> parseModifierP(NumericExpressionNode *lhs);

	// ( numeric_expression )
	// NUMBER
	// IDENTIFIER
	ParseResult<NumericExpressionNode> parsePrimary();
private:
	void dropStatement();
private:
	LithiumTokenizer tokenizer;
	std::vector<std::string> errors;

	int depth = 0;
};