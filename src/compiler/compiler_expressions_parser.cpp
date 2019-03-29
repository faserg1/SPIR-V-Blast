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

std::shared_ptr<CompilerNode> CompilerExpressionEndParser::end()
{
	auto expressionEnd = std::make_shared<CompilerExpressionEnd>();
	return std::make_shared<CompilerNode>(ECompilerNodeType::Expression, expressionEnd);
}

bool CompilerFunctionParametersStartParser::tryVisit(const std::string &expression)
{
	return expression == "(";
}

bool CompilerFunctionParametersStartParser::next()
{
	return false;
}

std::shared_ptr<CompilerNode> CompilerFunctionParametersStartParser::end()
{
	auto expressionEnd = std::make_shared<CompilerFunctionParametersStart>();
	return std::make_shared<CompilerNode>(ECompilerNodeType::Expression, expressionEnd);
}

bool CompilerFunctionParametersSeparatorParser::tryVisit(const std::string &expression)
{
	return expression == ",";
}

bool CompilerFunctionParametersSeparatorParser::next()
{
	return false;
}

std::shared_ptr<CompilerNode> CompilerFunctionParametersSeparatorParser::end()
{
	auto expressionEnd = std::make_shared<CompilerFunctionParametersSeparator>();
	return std::make_shared<CompilerNode>(ECompilerNodeType::Expression, expressionEnd);
}

bool CompilerFunctionParametersEndParser::tryVisit(const std::string &expression)
{
	return expression == ")";
}

bool CompilerFunctionParametersEndParser::next()
{
	return false;
}

std::shared_ptr<CompilerNode> CompilerFunctionParametersEndParser::end()
{
	auto expressionEnd = std::make_shared<CompilerFunctionParametersEnd>();
	return std::make_shared<CompilerNode>(ECompilerNodeType::Expression, expressionEnd);
}
