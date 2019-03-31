#include "compiler_expressions.hpp"

BlastExpression::BlastExpression(EBlastExpressionType type) :
	type_(type)
{
}

EBlastExpressionType BlastExpression::getType() const
{
	return type_;
}

BlastExpressionEnd::BlastExpressionEnd() :
	BlastExpression(EBlastExpressionType::End)
{
}

BlastFunctionParametersStart::BlastFunctionParametersStart() :
	BlastExpression(EBlastExpressionType::FunctionParametersStart)
{
}

BlastFunctionParametersSeparator::BlastFunctionParametersSeparator() :
	BlastExpression(EBlastExpressionType::FunctionParametersSeparator)
{
}

BlastFunctionParametersEnd::BlastFunctionParametersEnd() :
	BlastExpression(EBlastExpressionType::FunctionParametersEnd)
{
}
