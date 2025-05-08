//***********************************************
// LithiumParser.cpp
//***********************************************

#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "LithiumParser.h"
#include "Error.h"
#include "Color.h"

#define fail() return {false}
#define accept(node) return {true, node}

#define ret(funcCall) \
{ \
	auto result = funcCall; \
	if (!result.isValid()) \
	{ \
		fail(); \
	} \
	accept(result.getNode()); \
}

LithiumParser::LithiumParser():
	tokenizer(),
	errors()
{
}

ParseResult<Node> LithiumParser::parse(const std::string &filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << red << "error" << white << ": could not open file '" << filename << "'" << std::endl;
		fail();
	}

	std::string line;
	std::string source;
	while (std::getline(file, line))
	{
		source += line + "\n";
	}

	file.close();
	return parseInternal(source, filename);
}

std::vector<std::string> LithiumParser::getErrors() const
{
	return errors;
}

Token LithiumParser::peekToken()
{
	return tokenizer.peekToken();
}

Token LithiumParser::nextToken()
{
	return tokenizer.nextToken();
}

ParseResult<Node> LithiumParser::parseInternal(const std::string &source, const std::string &filename)
{
	tokenizer = LithiumTokenizer(source, filename);

	auto result = parseProgram();
	if (!result.isValid())
	{
		fail();
	}

	accept(result.getNode());
}

// program -> statements
ParseResult<ProgramNode> LithiumParser::parseProgram()
{
	auto statements = parseStatements();
	if (!statements.isValid())
	{
		fail();
	}

	accept(new ProgramNode(statements.getNode()));
}

// statements -> statement statements
//             | nothing
ParseResult<StatementsNode> LithiumParser::parseStatements()
{
	StatementsNode *statements = new StatementsNode();

	while (true)
	{
		auto statement = parseStatement();
		Token token = peekToken();

		if (!statement.isValid())
		{
			if (token == END || token == '}')
			{
				break;
			}
		}

		statements->addStatement(statement.getNode());
	}

	accept(statements);
}

// statement -> single_statement ;
//            | block
//            | control_statement
//            | func_decl
//            | ;
ParseResult<StatementNode> LithiumParser::parseStatement()
{
	static int blockDepth = 0;
	Token token = peekToken();

	if (token == END)
	{
		return false;
	}

	if (blockDepth > 0 && token == '}')
	{
		return false;
	}

	if (token == '{')
	{
		blockDepth++;

		auto block = parseBlock(); 
		if (!block.isValid()) 
		{ 
			blockDepth--;
			fail(); 
		}

		blockDepth--;
		accept(block.getNode());
	}
	else if (token == FOR || token == WHILE || token == IF)
	{
		ret(parseControlStatement());
	}
	else if (token == FN)
	{
		ret(parseFuncDecl());
	}

	auto statement = parseSingleStatement();
	if (!statement.isValid())
	{
		fail();
	}

	token = peekToken();

	if (token != ';')
	{
		expected("';'", token);
		fail();
	}
	nextToken();

	accept(statement.getNode());
}

// statement_list -> single_statement statement_list'
ParseResult<StatementListNode> LithiumParser::parseStatementList()
{
	auto statement = parseSingleStatement();
	if (!statement.isValid())
	{
		fail();
	}

	auto statementListP = parseStatementListP(statement.getNode());
	if (!statementListP.isValid())
	{
		fail();
	}
	
	accept(statementListP.getNode());
}

// statement_list' -> , single_statement statement_list'
//                  | nothing
ParseResult<StatementListNode> LithiumParser::parseStatementListP(StatementNode *lhs)
{
	Token token = peekToken();
	if (token != ',')
	{
		accept(new StatementListNode(lhs));
	}
	nextToken();

	auto statement = parseSingleStatement();
	if (!statement.isValid())
	{
		fail();
	}

	auto statementListP = parseStatementListP(statement.getNode());
	if (!statementListP.isValid())
	{
		fail();
	}

	accept(statementListP.getNode());
}

ParseResult<StatementNode> LithiumParser::parseControlStatement()
{
	Token token = peekToken();

    if (token == FOR)
	{
		ret(parseForStatement());
	}
	else if (token == WHILE)
	{
		ret(parseWhileStatement());
	}
	else if (token == IF)
	{
		ret(parseIfStatement());
	}

	fail();
}

// single_statement -> asm_statement
// 				     | expression
//                   | decl
//                   | ;
ParseResult<StatementNode> LithiumParser::parseSingleStatement()
{
	Token token = peekToken();

	if (token == LET)
	{
		ret(parseVarDecl());
	}
	else if (token == ASM)
	{
		ret(parseAsmStatement());
	}
	else if (token == RETURN)
	{
		ret(parseReturnStatement());
	}
	if (token == PRINT)
	{
		nextToken();

		token = peekToken();
		if (token != '(')
		{
			expected("'('", token);
			dropStatement();
			fail();
		}
		nextToken();
		
		auto expression = parseExpression();
		if (!expression.isValid())
		{
			fail();
		}

		token = peekToken();
		if (token != ')')
		{
			expected("')'", token);
			dropStatement();
			fail();
		}
		nextToken();

		accept(new PrintStatementNode(expression.getNode()));
	}
	else if (token == ';')
	{
		accept(nullptr);
	}

	ret(parseExpression());
}

ParseResult<ReturnStatementNode> LithiumParser::parseReturnStatement()
{
	Token token = peekToken();
	if (token != RETURN)
	{
		expected("'return'", token);
		dropStatement();
		fail();
	}
	nextToken();

	token = peekToken();

	if (token == ';')
	{
		accept(new ReturnStatementNode());
	}

	auto expression = parseExpression();
	if (!expression.isValid())
	{
		fail();
	}

	accept(new ReturnStatementNode(expression.getNode()));
}

// var_decl -> LET IDENTIFIER = expression
ParseResult<VarDeclNode> LithiumParser::parseVarDecl()
{
    Token token = peekToken();

	if (token != LET)
	{
		expected("'let'", token);
		dropStatement();
		fail();
	}
	nextToken();

	bool isConst = token.getText() == "const";

	token = peekToken();

	if (token != IDENTIFIER)
	{
		expected("an identifier", token);
		dropStatement();
		fail();
	}
	nextToken();

	Token identifier = token;
	token = peekToken();

	if (token != '=')
	{
		expected("'='", token);
		dropStatement();
		fail();
	}
	nextToken();

	auto expression = parseExpression();
	if (!expression.isValid())
	{
		fail();
	}

	accept(new VarDeclNode(identifier, expression.getNode(), isConst));
}

// func_decl -> FN IDENTIFIER ( param_list ) statement
ParseResult<FuncDeclNode> LithiumParser::parseFuncDecl()
{
	Token token = peekToken();
	if (token != FN)
	{
		expected("'fn'", token);
		dropStatement();
		fail();
	}
	nextToken();

	token = peekToken();
	if (token != IDENTIFIER)
	{
		expected("an identifier", token);
		dropStatement();
		fail();
	}
	nextToken();

	Token identifier = token;
	token = peekToken();
	if (token != '(')
	{
		expected("'('", token);
		dropStatement();
		fail();
	}
	nextToken();

	// Increase the scope for the function parameters
	// and body
	global::symbolTable.increaseScope();

	auto paramList = parseParamList();
	if (!paramList.isValid())
	{
		fail();
	}

	token = peekToken();
	if (token != ')')
	{
		expected("')'", token);
		dropStatement();
		fail();
	}
	nextToken();

	auto statement = parseStatement();
	if (!statement.isValid())
	{
		fail();
	}

	// Decrease the scope for the function parameters
	// and body
	global::symbolTable.decreaseScope();

	Symbol *symbol = global::symbolTable.lookupLocal(identifier.getText());
	// The symbol is free to use for the function declaration
	if (!symbol)
	{
		accept(new FuncDeclNode(identifier, paramList.getNode(), statement.getNode()));
	}

	// The symbol exists but is not a function
	if (!symbol->getDecl()->isFunction())
	{
		accept(new FuncDeclNode(identifier, paramList.getNode(), statement.getNode()));
	}

	// The symbol exists and is a function
	FuncDeclNode *funcDecl = static_cast<FuncDeclNode *>(symbol->getDecl());
	if (funcDecl->hasBody())
	{
		error("function '" + identifier.getText() + "' is already defined here: " + symbol->getToken().getLocation().toString(), identifier);
		fail();
	}

	// Compare the parameters of the existing function with the parsed parameters
	int existingParamCount = funcDecl->getParamList()->getParamCount();
	int newParamCount = paramList.getNode()->getParamCount();

	if (existingParamCount != newParamCount)
	{
		error("function '" + identifier.getText() + "' has a different number of parameters here: " + symbol->getToken().getLocation().toString(), identifier);
		fail();
	}

	bool hadParamError = false;

	for (int i = 0; i < existingParamCount; ++i)
	{
		Token existingParamToken = funcDecl->getParamList()->getParam(i)->getSymbol()->getToken();
		Token newParamToken = paramList.getNode()->getParam(i)->getSymbol()->getToken();
		std::string existingText = existingParamToken.getText();
		std::string existingLocationString = existingParamToken.getLocation().toString();
		std::string newText = newParamToken.getText();
		std::string newLocationString = newParamToken.getLocation().toString();

		if (existingText != newText)
		{
			// The parameter names are different
			error("function '" + identifier.getText() + "' has different parameter names here: " + existingLocationString, newParamToken);
			hadParamError = true;
		}
	}

	if (hadParamError)
	{
		fail();
	}

	// reassign the function body and parameter list
	funcDecl->setParamList(paramList.getNode());
	funcDecl->setBody(statement.getNode());

	// accept the parsed function declaration but don't create a new node for it
	accept(nullptr);
}

//param_list -> param param_list'
ParseResult<ParamListNode> LithiumParser::parseParamList()
{
	Token token = peekToken();

	if (token == ')')
	{
		accept(new ParamListNode());
	}

	auto param = parseParam();
	if (!param.isValid())
	{
		fail();
	}

	auto paramListP = parseParamListP(param.getNode());
	if (!paramListP.isValid())
	{
		fail();
	}

	accept(paramListP.getNode());
}

//param_list' -> , param_list
//             | nothing
ParseResult<ParamListNode> LithiumParser::parseParamListP(ParamNode *lhs)
{
	Token token = peekToken();
	if (token != ',')
	{
		accept(new ParamListNode(lhs));
	}

	ParamListNode *list = new ParamListNode(lhs);

	nextToken();

	auto paramList = parseParamList();
	if (!paramList.isValid())
	{
		fail();
	}

	list->addAllParams(paramList.getNode());

	accept(list);
}

ParseResult<ParamNode> LithiumParser::parseParam()
{
	Token token = peekToken();

	if (token != IDENTIFIER)
	{
		expected("an identifier", token);
		dropStatement();
		fail();
	}

	nextToken();

	accept(new ParamNode(token));
}

// block -> { statements }
ParseResult<BlockNode> LithiumParser::parseBlock()
{
	Token token = peekToken();
	if (token != '{')
	{
		expected("'{'", token);
		fail();
	}
	nextToken();

	depth++;
	global::symbolTable.increaseScope();

	auto statements = parseStatements();
	if (!statements.isValid())
	{
		fail();
	}

	token = peekToken();
	if (token != '}')
	{
		expected("'}'", token);
		fail();
	}
	nextToken();

	depth--;
	global::symbolTable.decreaseScope();

	accept(new BlockNode(statements.getNode()));
}

// if_statement -> IF ( numeric_expression ) statement if_statement'
ParseResult<IfStatementNode> LithiumParser::parseIfStatement()
{
	Token token = peekToken();
	if (token != IF)
	{
		expected("IF", token);
		fail();
	}

	nextToken();

	token = peekToken();
	if (token != '(')
	{
		expected("'('", token);
		fail();
	}

	nextToken();

	auto condition = parseNumericExpression();
	if (!condition.isValid())
	{
		fail();
	}

	token = peekToken();
	if (token != ')')
	{
		expected("')'", token);
		fail();
	}

	nextToken();

	auto statement = parseStatement();
	if (!statement.isValid())
	{
		fail();
	}

	auto ifStatementP = parseIfStatementP();
	if (!ifStatementP.isValid())
	{
		fail();
	}

	accept(new IfStatementNode(condition.getNode(), statement.getNode(), ifStatementP.getNode()));
}

// if_statement' -> ELSE statement
//                | nothing
ParseResult<StatementNode> LithiumParser::parseIfStatementP()
{
	Token token = peekToken();
	if (token != ELSE)
	{
		accept();
	}

	nextToken();

	auto statement = parseStatement();
	if (!statement.isValid())
	{
		fail();
	}

	accept(statement.getNode());
}

// while_statement -> WHILE ( numeric_expression ) statement
ParseResult<WhileStatementNode> LithiumParser::parseWhileStatement()
{
	Token token = peekToken();
	if (token != WHILE)
	{
		expected("'while'", token);
		fail();
	}

	nextToken();

	token = peekToken();
	if (token != '(')
	{
		expected("'('", token);
		fail();
	}

	nextToken();

	auto condition = parseNumericExpression();
	if (!condition.isValid())
	{
		fail();
	}

	token = peekToken();
	if (token != ')')
	{
		expected("')'", token);
		fail();
	}

	nextToken();

	auto statement = parseStatement();
	if (!statement.isValid())
	{
		fail();
	}

	accept(new WhileStatementNode(condition.getNode(), statement.getNode()));
}

// for -> FOR ( statement_list ; numeric_expression ; statement_list ) statement
ParseResult<ForStatementNode> LithiumParser::parseForStatement()
{
	Token token = peekToken();
	if (token != FOR)
	{
		expected("'for'", token);
		dropStatement();
		fail();
	}
	nextToken();

	token = peekToken();
	if (token != '(')
	{
		expected("'('", token);
		dropStatement();
		fail();
	}
	nextToken();

	// Increase the scope for the for loop
	// and body
	global::symbolTable.increaseScope();

	auto init = parseStatementList();
	if (!init.isValid())
	{
		fail();
	}

	token = peekToken();
	if (token != ';')
	{
		expected("';'", token);
		dropStatement();
		fail();
	}
	nextToken();

	auto condition = parseNumericExpression();
	if (!condition.isValid())
	{
		fail();
	}

	token = peekToken();
	if (token != ';')
	{
		expected("';'", token);
		dropStatement();
		fail();
	}
	nextToken();

	auto increment = parseStatementList();
	if (!increment.isValid())
	{
		fail();
	}

	token = peekToken();
	if (token != ')')
	{
		expected("')'", token);
		dropStatement();
		fail();
	}
	nextToken();

	auto statement = parseStatement();
	if (!statement.isValid())
	{
		fail();
	}

	// Decrease the scope for the for loop
	global::symbolTable.decreaseScope();

	accept(new ForStatementNode(init.getNode(), condition.getNode(), increment.getNode(), statement.getNode()));
}

// asm_statement -> ASM ( string_expression )
ParseResult<AsmStatementNode> LithiumParser::parseAsmStatement()
{
	Token token = peekToken();
	if (token != ASM)
	{
		expected("'asm'", token);
		dropStatement();
		fail();
	}

	nextToken();

	token = peekToken();
	if (token != '(')
	{
		expected("'('", token);
		dropStatement();
		fail();
	}

	nextToken();

	auto stringExpression = parseStringExpression();
	if (!stringExpression.isValid())
	{
		fail();
	}

	token = peekToken();
	if (token != ')')
	{
		expected("')'", token);
		dropStatement();
		fail();
	}

	nextToken();

	return {true, new AsmStatementNode(stringExpression.getNode())};
}

// expression -> numeric_expression
// 			   | string_expression
ParseResult<ExpressionNode> LithiumParser::parseExpression()
{
	Token token = peekToken();

	if (token == STRING || token == STR)
	{
		ret(parseStringExpression());
	}

	ret(parseNumericExpression()); // TODO: look into if this should check the first sets
}

// string_expression -> STRING string_expression'
//                    | STR ( numeric_expression ) string_expression'
ParseResult<StringExpressionNode> LithiumParser::parseStringExpression()
{
	Token token = peekToken();

	if (token == STR)
	{
		nextToken();

		token = peekToken();
		if (token != '(')
		{
			expected("'('", token);
			dropStatement();
			fail();
		}
		nextToken();

		auto expression = parseNumericExpression();
		if (!expression.isValid())
		{
			fail();
		}

		token = peekToken();
		if (token != ')')
		{
			expected("')'", token);
			dropStatement();
			fail();
		}
		nextToken();

		StringConversionNode *conversion = new StringConversionNode(expression.getNode());

		auto stringExpressionP = parseStringExpressionP(conversion);
		if (!stringExpressionP.isValid())
		{
			fail();
		}

		accept(stringExpressionP.getNode());
	}

	if (token != STRING)
	{
		expected("a string", token);
		dropStatement();
		fail();
	}

	nextToken();

	auto stringExpressionP = parseStringExpressionP(new StringExpressionNode(token));
	if (!stringExpressionP.isValid())
	{
		fail();
	}

	accept(stringExpressionP.getNode());
}

// string_expression' -> + expression string_expression'
//                     | nothing
ParseResult<StringExpressionNode> LithiumParser::parseStringExpressionP(StringExpressionNode *lhs)
{
	Token token = peekToken();
	if (token != '+')
	{
		accept(lhs);
	}

	nextToken();

	auto expression = parseExpression();
	if (!expression.isValid())
	{
		fail();
	}

	auto stringExpressionP = parseStringExpressionP(new ConcatNode(lhs, expression.getNode()));
	if (!stringExpressionP.isValid())
	{
		fail();
	}

	accept(stringExpressionP.getNode());
}

// numeric_expression -> assignment
ParseResult<NumericExpressionNode> LithiumParser::parseNumericExpression()
{
	ret(parseAssignment());
}

// assignment -> optional assignment'
ParseResult<NumericExpressionNode> LithiumParser::parseAssignment()
{
	auto optional = parseOptional();
	if (!optional.isValid())
	{
		fail();
	}

	if (!optional.getNode()->isVariable())
	{
		accept(optional.getNode());
	}

	VariableExpressionNode *lhs = dynamic_cast<VariableExpressionNode*>(optional.getNode());

	auto assignmentP = parseAssignmentP(lhs);
	if (!assignmentP.isValid())
	{
		fail();
	}

	accept(assignmentP.getNode());
}

// assignment' -> = expression
//              | nothing
ParseResult<NumericExpressionNode> LithiumParser::parseAssignmentP(VariableExpressionNode *lhs)
{
	Token token = peekToken();
	if (token != '=')
	{
		accept(lhs);
	}
	nextToken();

	if (lhs->isConst())
	{
		error("cannot assign to constant: " + lhs->getName() + " defined here: " + lhs->getSymbol()->getDecl()->getToken().getLocation().toString(), lhs->getToken());
		fail();
	}

	auto expression = parseExpression();
	if (!expression.isValid())
	{
		fail();
	}

	accept(new AssignNode(lhs, expression.getNode()));
}

// optional -> compound optional'
ParseResult<NumericExpressionNode> LithiumParser::parseOptional()
{
	auto compound = parseCompound();
	if (!compound.isValid())
	{
		fail();
	}

	auto optionalP = parseOptionalP(compound.getNode());
	if (!optionalP.isValid())
	{
		fail();
	}

	accept(optionalP.getNode());
}

// optional' -> || compound optional'
// 			  | nothing
ParseResult<NumericExpressionNode> LithiumParser::parseOptionalP(NumericExpressionNode *lhs)
{
	Token token = peekToken();
	if (token != OR)
	{
		accept(lhs);
	}

	nextToken();

	auto compound = parseCompound();
	if (!compound.isValid())
	{
		fail();
	}

	auto optionalP = parseOptionalP(new BinaryExpressionNode(lhs, OR, compound.getNode()));
	if (!optionalP.isValid())
	{
		fail();
	}

	accept(optionalP.getNode());
}

// compound -> equality compound'
ParseResult<NumericExpressionNode> LithiumParser::parseCompound()
{
	auto equality = parseEquality();
	if (!equality.isValid())
	{
		fail();
	}

	auto compoundP = parseCompoundP(equality.getNode());
	if (!compoundP.isValid())
	{
		fail();
	}

	accept(compoundP.getNode());
}

// compound' -> && equality compound'
//			  | nothing
ParseResult<NumericExpressionNode> LithiumParser::parseCompoundP(NumericExpressionNode *lhs)
{
	Token token = peekToken();
	if (token != AND)
	{
		accept(lhs);
	}

	nextToken();

	auto equality = parseEquality();
	if (!equality.isValid())
	{
		fail();
	}

	auto compoundP = parseCompoundP(new BinaryExpressionNode(lhs, AND, equality.getNode()));
	if (!compoundP.isValid())
	{
		fail();
	}

	accept(compoundP.getNode());
}

// equality -> comparison equality'
ParseResult<NumericExpressionNode> LithiumParser::parseEquality()
{
	auto comparison = parseComparison();
	if (!comparison.isValid())
	{
		fail();
	}

	auto equalityP = parseEqualityP(comparison.getNode());
	if (!equalityP.isValid())
	{
		fail();
	}

	accept(equalityP.getNode());
}

// equality' -> == comparison equality'
//            | != comparison equality'
//            | nothing
ParseResult<NumericExpressionNode> LithiumParser::parseEqualityP(NumericExpressionNode *lhs)
{
	Token token = peekToken();
	if (token != EQUAL && token != NOT_EQUAL)
	{
		accept(lhs);
	}

	nextToken();

	auto comparison = parseComparison();
	if (!comparison.isValid())
	{
		fail();
	}

	auto equalityP = parseEqualityP(new BinaryExpressionNode(lhs, token, comparison.getNode()));
	if (!equalityP.isValid())
	{
		fail();
	}

	accept(equalityP.getNode());
}

// comparison -> addit comparison'
ParseResult<NumericExpressionNode> LithiumParser::parseComparison()
{
	auto addit = parseAddit();
	if (!addit.isValid())
	{
		fail();
	}

	auto comparisonP = parseComparisonP(addit.getNode());
	if (!comparisonP.isValid())
	{
		fail();
	}

	accept(comparisonP.getNode());
}

// comparison' -> < addit comparison'
// 				| > addit comparison'
// 				| <= addit comparison'
// 				| >= addit comparison'
ParseResult<NumericExpressionNode> LithiumParser::parseComparisonP(NumericExpressionNode *lhs)
{
	Token token = peekToken();
	if (token != '<' && token != '>' && token != LESS_OR_EQUAL && token != GREATER_OR_EQUAL)
	{
		accept(lhs);
	}

	nextToken();

	auto addit = parseAddit();
	if (!addit.isValid())
	{
		fail();
	}

	auto comparisonP = parseComparisonP(new BinaryExpressionNode(lhs, token, addit.getNode()));
	if (!comparisonP.isValid())
	{
		fail();
	}

	accept(comparisonP.getNode());
}

// addit -> term addit'
ParseResult<NumericExpressionNode> LithiumParser::parseAddit()
{
	auto term = parseTerm();
	if (!term.isValid())
	{
		fail();
	}

	auto additP = parseAdditP(term.getNode());
	if (!additP.isValid())
	{
		fail();
	}

	accept(additP.getNode());
}

// addit' -> + term addit'
//         | - term addit'
//         | nothing
ParseResult<NumericExpressionNode> LithiumParser::parseAdditP(NumericExpressionNode *lhs)
{
	Token token = peekToken();
	if (token != '+' && token != '-')
	{
		accept(lhs);
	}

	nextToken();

	auto term = parseTerm();
	if (!term.isValid())
	{
		fail();
	}

	auto additP = parseAdditP(new BinaryExpressionNode(lhs, token, term.getNode()));
	if (!additP.isValid())
	{
		fail();
	}

	accept(additP.getNode());
}


// term -> exponent term'
ParseResult<NumericExpressionNode> LithiumParser::parseTerm()
{
	auto exponent = parseExponent();
	if (!exponent.isValid())
	{
		fail();
	}

	auto termP = parseTermP(exponent.getNode());
	if (!termP.isValid())
	{
		fail();
	}

	accept(termP.getNode());
}

// term' -> * exponent
//        | / exponent
//        | % exponent
//        | nothing
ParseResult<NumericExpressionNode> LithiumParser::parseTermP(NumericExpressionNode *lhs)
{
	Token token = peekToken();
	if (token != '*' && token != '/' && token != '%')
	{
		accept(lhs);
	}

	nextToken();

	auto exponent = parseExponent();
	if (!exponent.isValid())
	{
		fail();
	}

	auto termP = parseTermP(new BinaryExpressionNode(lhs, token, exponent.getNode()));
	if (!termP.isValid())
	{
		fail();
	}

	accept(termP.getNode());
}

// exponent -> fact exponent'
ParseResult<NumericExpressionNode>  LithiumParser::parseExponent()
{
	auto fact = parseFact();
	if (!fact.isValid())
	{
		fail();
	}

	auto exponentP = parseExponentP(fact.getNode());
	if (!exponentP.isValid())
	{
		fail();
	}

	accept(exponentP.getNode());
}

// exponent' -> ^ fact exponent'
// 			  | nothing
ParseResult<NumericExpressionNode> LithiumParser::parseExponentP(NumericExpressionNode *lhs)
{
	Token token = peekToken();
	if (token != '^')
	{
		accept(lhs);
	}

	nextToken();

	auto fact = parseFact();
	if (!fact.isValid())
	{
		fail();
	}

	auto exponentP = parseExponentP(new BinaryExpressionNode(lhs, '^', fact.getNode()));
	if (!exponentP.isValid())
	{
		fail();
	}

	accept(exponentP.getNode());
}

// fact -> - factorial
//       | factorial
ParseResult<NumericExpressionNode> LithiumParser::parseFact()
{
	Token token = peekToken();
	if (token == '-')
	{
		nextToken();

		auto factorial = parseFactorial();
		if (!factorial.isValid())
		{
			fail();
		}

		accept(new UnaryExpressionNode(factorial.getNode(), '-'));
	}

	ret(parseFactorial());
}

// factorial -> modifier factorial'
ParseResult<NumericExpressionNode> LithiumParser::parseFactorial()
{
	auto modifier = parseModifier();
	if (!modifier.isValid())
	{
		fail();
	}

	auto factorialP = parseFactorialP(modifier.getNode());
	if (!factorialP.isValid())
	{
		fail();
	}

	accept(factorialP.getNode());
}

// factorial' -> ! factorial'
//             | nothing
ParseResult<NumericExpressionNode> LithiumParser::parseFactorialP(NumericExpressionNode *lhs)
{
	Token token = peekToken();
	if (token != '!')
	{
		accept(lhs);
	}

	nextToken();

	auto factorialP = parseFactorialP(new UnaryExpressionNode(lhs, '!'));
	if (!factorialP.isValid())
	{
		fail();
	}

	accept(factorialP.getNode());
}

// modifier -> primary modifier'
ParseResult<NumericExpressionNode> LithiumParser::parseModifier()
{
	auto primary = parsePrimary();
	if (!primary.isValid())
	{
		fail();
	}

	auto modifierP = parseModifierP(primary.getNode());
	if (!modifierP.isValid())
	{
		fail();
	}

	accept(modifierP.getNode());
}

// modifier' -> ++ modifier'
//            | -- modifier'
//            | nothing
ParseResult<NumericExpressionNode> LithiumParser::parseModifierP(NumericExpressionNode *lhs)
{
	Token token = peekToken();
	if (token != INCREMENT && token != DECREMENT)
	{
		accept(lhs);
	}

	nextToken();

	auto modifierP = parseModifierP(new UnaryExpressionNode(lhs, token));
	if (!modifierP.isValid())
	{
		fail();
	}

	accept(modifierP.getNode());
}

// primary -> ( numeric_expression )
//          | NUMBER
//          | IDENTIFIER primary'
ParseResult<NumericExpressionNode> LithiumParser::parsePrimary()
{
	Token token = peekToken();

	if (token == '(')
	{
		nextToken();
		auto expression = parseNumericExpression();
		if (!expression.isValid())
		{
			fail();
		}

		token = peekToken();
		if (token != ')')
		{
			expected("')'", token);
			dropStatement();
			fail();
		}

		nextToken();

		accept(expression.getNode());
	}

	if (token == NUMBER)
	{
		nextToken();
		int value = std::stoi(token.getText());
		accept(new IntExpressionNode(value));
	}

	if (token == IDENTIFIER)
	{
		nextToken();
		Token identifier = token;

		auto primaryP = parsePrimaryP(identifier);
		if (!primaryP.isValid())
		{
			fail();
		}

		accept(primaryP.getNode());
	}

	expected("an expression", token);
	dropStatement();
	fail();
}

// primary' -> ( arg_list )
//           | nothing
ParseResult<NumericExpressionNode> LithiumParser::parsePrimaryP(const Token &identifier)
{
	Token token = peekToken();
	if (token != '(')
	{
		accept(new VariableExpressionNode(identifier));
	}

	nextToken();

	auto argList = parseArgList();
	if (!argList.isValid())
	{
		fail();
	}

	token = peekToken();
	if (token != ')')
	{
		expected("')'", token);
		dropStatement();
		fail();
	}

	nextToken();

	accept(new CallNode(identifier, argList.getNode()));
}

// arg_list -> arg arg_list'
ParseResult<ArgListNode> LithiumParser::parseArgList()
{
	Token token = peekToken();

	// if the first token is a ')', then we have an empty arg list
	if (token == ')')
	{
		accept(new ArgListNode());
	}

	auto arg = parseArg();
	if (!arg.isValid())
	{
		accept(new ArgListNode());
	}

	auto argListP = parseArgListP(arg.getNode());
	if (!argListP.isValid())
	{
		fail();
	}

	accept(argListP.getNode());
}

// arg_list' -> , arg_list
//            | nothing
ParseResult<ArgListNode> LithiumParser::parseArgListP(ArgNode *lhs)
{
	ArgListNode *list = new ArgListNode(lhs);
	Token token = peekToken();
	if (token != ',')
	{
		accept(list);
	}
	nextToken();

	auto argListP = parseArgList();
	if (!argListP.isValid())
	{
		fail();
	}

	list->addAllArgs(argListP.getNode());

	accept(list);
}

// arg -> expression
ParseResult<ArgNode> LithiumParser::parseArg()
{
	auto expression = parseExpression();
	if (!expression.isValid())
	{
		fail();
	}

	accept(new ArgNode(expression.getNode()));
}

// drop to the start of the next statement
void LithiumParser::dropStatement()
{
	Token token = peekToken();
	while (token != ';' && token != END && (token != '}' || depth == 0))
	{
		//fprintf(stderr, "dropped: %s\n", token.getText().c_str());
		token = nextToken();
	}

	if (token != '}')
	{
		nextToken();
	}
}
