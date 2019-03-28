#include "compiler_expressions_parser.hpp"
#include "compiler_expressions.hpp"
#include "compiler_nodes.hpp"

bool CompilerExpressionEndParser::tryVisit(const std::string & expression)
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
