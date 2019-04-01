#include "parser_expressions_parser.hpp"
#include "parser_expressions.hpp"
#include "parser_nodes.hpp"

bool BlastExpressionEndParser::tryVisit(const std::string &expression)
{
	return expression == ";";
}

bool BlastExpressionEndParser::next()
{
	return false;
}

std::shared_ptr<ParserNode> BlastExpressionEndParser::end(EParserState state)
{
	auto expressionEnd = std::make_shared<BlastExpressionEnd>();
	return std::make_shared<ParserNode>(EParserNodeType::Expression, expressionEnd, state);
}

bool BlastFunctionParametersStartParser::tryVisit(const std::string &expression)
{
	return expression == "(";
}

bool BlastFunctionParametersStartParser::next()
{
	return false;
}

std::shared_ptr<ParserNode> BlastFunctionParametersStartParser::end(EParserState state)
{
	auto expressionEnd = std::make_shared<BlastFunctionParametersStart>();
	return std::make_shared<ParserNode>(EParserNodeType::Expression, expressionEnd, state);
}

bool BlastFunctionParametersSeparatorParser::tryVisit(const std::string &expression)
{
	return expression == ",";
}

bool BlastFunctionParametersSeparatorParser::next()
{
	return false;
}

std::shared_ptr<ParserNode> BlastFunctionParametersSeparatorParser::end(EParserState state)
{
	auto expressionEnd = std::make_shared<BlastFunctionParametersSeparator>();
	return std::make_shared<ParserNode>(EParserNodeType::Expression, expressionEnd, state);
}

bool BlastFunctionParametersEndParser::tryVisit(const std::string &expression)
{
	return expression == ")";
}

bool BlastFunctionParametersEndParser::next()
{
	return false;
}

std::shared_ptr<ParserNode> BlastFunctionParametersEndParser::end(EParserState state)
{
	auto expressionEnd = std::make_shared<BlastFunctionParametersEnd>();
	return std::make_shared<ParserNode>(EParserNodeType::Expression, expressionEnd, state);
}
