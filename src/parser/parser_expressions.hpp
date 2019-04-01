#ifndef PARSER_EXPRESSIONS
#define PARSER_EXPRESSIONS

enum class EBlastExpressionType
{
	End,
	FunctionParametersStart,
	FunctionParametersSeparator,
	FunctionParametersEnd,
	ExpressionBodyStart,
	ExpressionBodyEnd,
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

class BlastExpressionBodyStart :
	public BlastExpression
{
public:
	BlastExpressionBodyStart();
};

class BlastExpressionBodyEnd :
	public BlastExpression
{
public:
	BlastExpressionBodyEnd();
};

#endif