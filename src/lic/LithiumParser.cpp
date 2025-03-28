//***********************************************
// LithiumParser.cpp
//***********************************************

#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>

#include "LithiumParser.h"
#include "Error.h"
#include "Color.h"

#define fail() return {false}
#define accept(x) return {true, x}

#define ret(x) \
{ \
	auto node = x; \
	if (!node.isValid()) \
	{ \
		fail(); \
	} \
	accept(node.getNode()); \
}

LithiumParser::LithiumParser():
	tokenizer(),
	errors()
{
}

ParseResult<Node> LithiumParser::parse(const std::string &source)
{
	std::ifstream file(source);
	// if the file is not open, treat the source as a string
	if (!file.is_open())
	{
		return parseInternal(source);
	}

	// if the file is open, treat the source as a filename
	// and read the file into a string
	std::string line;
	std::string content;
	while (std::getline(file, line))
	{
		content += line + "\n";
	}

	file.close();
	return parseInternal(content, source);
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
//            | for_statement
//            | while_statement
//            | if_statement
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
			return false; 
		}

		blockDepth--;
		accept(block.getNode());
	}
	else if (token == FOR)
	{
		ret(parseForStatement())
	}
	else if (token == WHILE)
	{
		ret(parseWhileStatement());
	}
	else if (token == IF)
	{
		ret(parseIfStatement());
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

// single_statement -> expression
//                   | print_statement
//                   | asm_statement
//                   | ;
ParseResult<StatementNode> LithiumParser::parseSingleStatement()
{
	Token token = peekToken();

	if (token == PRINT)
	{
		ret(parsePrintStatement());
	}
	else if (token == ASM)
	{
		ret(parseAsmStatement());
	}
	else if (token == ';')
	{
		accept(nullptr);
	}

	ret(parseExpression());
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

	depth++;

	nextToken();

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

	depth--;

	nextToken();

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

// print_statement -> PRINT ( expression )
ParseResult<PrintStatementNode> LithiumParser::parsePrintStatement()
{
	Token token = peekToken();
	if (token != PRINT)
	{
		expected("'print'", token);
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

// expression -> numeric_expression
// 			   | string_expression
ParseResult<ExpressionNode> LithiumParser::parseExpression()
{
	Token token = peekToken();

	if (token == STRING)
	{
		ret(parseStringExpression());
	}

	ret(parseNumericExpression()); // TODO: look into if this should check the first sets
}

// string_expression -> STRING string_expression'
ParseResult<StringExpressionNode> LithiumParser::parseStringExpression()
{
	Token token = peekToken();
	if (token != STRING)
	{
		expected("a string", token);
		dropStatement();
		fail();
	}

	nextToken();

	auto stringExpressionP = parseStringExpressionP(new StringExpressionNode(token.getText()));
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

// ( numeric_expression )
// NUMBER
// IDENTIFIER
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
		accept(new VariableExpressionNode(token));
	}

	expected("an expression", token);
	dropStatement();
	fail();
}

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
