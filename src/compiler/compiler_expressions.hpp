#ifndef COMPILER_EXPRESSIONS
#define COMPILER_EXPRESSIONS

enum class EBlastExpressionType
{
	End,
	FunctionParametersStart,
	FunctionParametersSeparator,
	FunctionParametersEnd,
};

class BlastExpression
{
protected:
	BlastExpression(EBlastExpressionType type);

	EBlastExpressionType getType() const;
private:
	const EBlastExpressionType type_;
};

class BlastExpressionEnd :
	public BlastExpression
{
public:
	BlastExpressionEnd();
};

class BlastFunctionParametersStart :
	public BlastExpression
{
public:
	BlastFunctionParametersStart();
};

class BlastFunctionParametersSeparator :
	public BlastExpression
{
public:
	BlastFunctionParametersSeparator();
};

class BlastFunctionParametersEnd :
	public BlastExpression
{
public:
	BlastFunctionParametersEnd();
};

#endif