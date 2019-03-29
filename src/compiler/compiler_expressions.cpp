#include "compiler_expressions.hpp"

CompilerExpression::CompilerExpression(ECompilerExpressionType type) :
	type_(type)
{
}

ECompilerExpressionType CompilerExpression::getType() const
{
	return type_;
}

CompilerExpressionEnd::CompilerExpressionEnd() :
	CompilerExpression(ECompilerExpressionType::End)
{
}

CompilerFunctionParametersStart::CompilerFunctionParametersStart() :
	CompilerExpression(ECompilerExpressionType::FunctionParametersStart)
{
}

CompilerFunctionParametersSeparator::CompilerFunctionParametersSeparator() :
	CompilerExpression(ECompilerExpressionType::FunctionParametersSeparator)
{
}

CompilerFunctionParametersEnd::CompilerFunctionParametersEnd() :
	CompilerExpression(ECompilerExpressionType::FunctionParametersEnd)
{
}
