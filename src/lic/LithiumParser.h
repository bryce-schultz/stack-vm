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

	ParseResult<Node> parse(const std::string &filename);
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
	// 		      | control_statement
	//            | ;
	ParseResult<StatementNode> parseStatement();
	
	// statement_list -> single_statement statement_list'
	ParseResult<StatementListNode> parseStatementList();

	// statement_list' -> , single_statement statement_list'
	//                  | nothing
	ParseResult<StatementListNode> parseStatementListP(StatementNode *lhs);

	//control_statement -> for_statement
    //                   | while_statement
    //                   | if_statement
	ParseResult<StatementNode> parseControlStatement();

	// single_statement -> asm_statement
	//                   | expression
	//                   | decl
	//                   | return_statement
	//                   | ;
	ParseResult<StatementNode> parseSingleStatement();

	// return_statement -> RETURN expression
	ParseResult<ReturnStatementNode> parseReturnStatement();

	// decl -> var_decl
	//       | func_decl
	ParseResult<StatementNode> parseDecl();

	// var_decl -> LET IDENTIFIER = expression
	ParseResult<VarDeclNode> parseVarDecl();

	// func_decl -> FN IDENTIFIER ( param_list ) statement
	ParseResult<FuncDeclNode> parseFuncDecl();

	//param_list -> param param_list'
	ParseResult<ParamListNode> parseParamList();

	//param_list' -> , param_list
    //             | nothing
	ParseResult<ParamListNode> parseParamListP(ParamNode *lhs);

	// param -> IDENTIFIER
	ParseResult<ParamNode> parseParam();

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

	// expression -> assignment
	ParseResult<ExpressionNode> parseExpression();

	// assignment -> optional assignment'
	ParseResult<ExpressionNode> parseAssignment();

	// assignment' -> = expression
	//              | nothing
	ParseResult<ExpressionNode> parseAssignmentP(VariableExpressionNode *lhs);

	// optional -> compound optional'
	ParseResult<ExpressionNode> parseOptional();

	// optional' -> || compound optional'
	// 			  | nothing
	ParseResult<ExpressionNode> parseOptionalP(ExpressionNode *lhs);

	// compound -> equality compound'
	ParseResult<ExpressionNode> parseCompound();

	// compound' -> && equality compound'
	//			  | nothing
	ParseResult<ExpressionNode> parseCompoundP(ExpressionNode *lhs);

	// equality -> comparison equality'
	ParseResult<ExpressionNode> parseEquality();

	// equality' -> == comparison equality'
	//            | != comparison equality'
	//            | nothing
	ParseResult<ExpressionNode> parseEqualityP(ExpressionNode *lhs);

	// comparison -> addit comparison'
	ParseResult<ExpressionNode> parseComparison();

	// comparison' -> < addit comparison'
	// 				| > addit comparison'
	// 				| <= addit comparison'
	// 				| >= addit comparison'
	ParseResult<ExpressionNode> parseComparisonP(ExpressionNode *lhs);

	// addit -> term addit'
	ParseResult<ExpressionNode> parseAddit();

	// addit' -> + term addit'
	//         | - term addit'
    //         | nothing
	ParseResult<ExpressionNode> parseAdditP(ExpressionNode *lhs);


	// term -> exponent term'
	ParseResult<ExpressionNode> parseTerm();

	// term' -> * exponent
	//        | / exponent
	//        | % exponent
	//        | nothing
	ParseResult<ExpressionNode> parseTermP(ExpressionNode *lhs);

	// exponent -> fact exponent'
	ParseResult<ExpressionNode>  parseExponent();

	// exponent' -> ^ fact exponent'
	// 			  | nothing
	ParseResult<ExpressionNode> parseExponentP(ExpressionNode *);

	// fact -> - factorial
	//       | factorial
	ParseResult<ExpressionNode> parseFact();

	// factorial -> primary factorial'
	ParseResult<ExpressionNode> parseFactorial();

	// factorial' -> ! factorial'
	//             | nothing
	ParseResult<ExpressionNode> parseFactorialP(ExpressionNode *lhs);

	// modifier -> primary modifier'
	ParseResult<ExpressionNode> parseModifier();

	// modifier' -> ++ modifier'
	//            | -- modifier'
	//            | nothing
	ParseResult<ExpressionNode> parseModifierP(ExpressionNode *lhs);

	// primary -> ( numeric_expression )
	//          | NUMBER
	//          | IDENTIFIER primary'
	ParseResult<ExpressionNode> parsePrimary();
	
	// primary' -> ( arg_list )
    //           | ϵ
	ParseResult<ExpressionNode> parsePrimaryP(const Token &identifier);

	// arg_list -> arg arg_list'
	ParseResult<ArgListNode> parseArgList();

	// arg_list' -> , arg_list
	//            | nothing
	ParseResult<ArgListNode> parseArgListP(ArgNode *lhs);
	
	// arg -> expression
	ParseResult<ArgNode> parseArg();
private:
	void dropStatement();
private:
	LithiumTokenizer tokenizer;
	std::vector<std::string> errors;

	int depth = 0;
};