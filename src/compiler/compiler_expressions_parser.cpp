#include "compiler_expressions_parser.hpp"
#include "compiler_expressions.hpp"
#include "compiler_nodes.hpp"

bool CompilerExpressionEndParser::tryVisit(const std::string &expression)
{
	return expression == ";";
}

bool CompilerExpressionEndParser::next()
{
	return false;
}

std::shared_ptr<CompilerNode> CompilerExpressionEndParser::end(ECompilerState state)
{
	auto expressionEnd = std::make_shared<CompilerExpressionEnd>();
	return std::make_shared<CompilerNode>(ECompilerNodeType::Expression, expressionEnd, state);
}

bool CompilerFunctionParametersStartParser::tryVisit(const std::string &expression)
{
	return expression == "(";
}

bool CompilerFunctionParametersStartParser::next()
{
	return false;
}

std::shared_ptr<CompilerNode> CompilerFunctionParametersStartParser::end(ECompilerState state)
{
	auto expressionEnd = std::make_shared<CompilerFunctionParametersStart>();
	return std::make_shared<CompilerNode>(ECompilerNodeType::Expression, expressionEnd, state);
}

bool CompilerFunctionParametersSeparatorParser::tryVisit(const std::string &expression)
{
	return expression == ",";
}

bool CompilerFunctionParametersSeparatorParser::next()
{
	return false;
}

std::shared_ptr<CompilerNode> CompilerFunctionParametersSeparatorParser::end(ECompilerState state)
{
	auto expressionEnd = std::make_shared<CompilerFunctionParametersSeparator>();
	return std::make_shared<CompilerNode>(ECompilerNodeType::Expression, expressionEnd, state);
}

bool CompilerFunctionParametersEndParser::tryVisit(const std::string &expression)
{
	return expression == ")";
}

bool CompilerFunctionParametersEndParser::next()
{
	return false;
}

std::shared_ptr<CompilerNode> CompilerFunctionParametersEndParser::end(ECompilerState state)
{
	auto expressionEnd = std::make_shared<CompilerFunctionParametersEnd>();
	return std::make_shared<CompilerNode>(ECompilerNodeType::Expression, expressionEnd, state);
}
