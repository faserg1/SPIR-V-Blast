#ifndef COMPILER_EXPRESSIONS
#define COMPILER_EXPRESSIONS

enum class ECompilerExpressionType
{
	End,
	FunctionParametersStart,
	FunctionParametersSeparator,
	FunctionParametersEnd,
};

class CompilerExpression
{
protected:
	CompilerExpression(ECompilerExpressionType type);

	ECompilerExpressionType getType() const;
private:
	const ECompilerExpressionType type_;
};

class CompilerExpressionEnd :
	public CompilerExpression
{
public:
	CompilerExpressionEnd();
};

class CompilerFunctionParametersStart :
	public CompilerExpression
{
public:
	CompilerFunctionParametersStart();
};

class CompilerFunctionParametersSeparator :
	public CompilerExpression
{
public:
	CompilerFunctionParametersSeparator();
};

class CompilerFunctionParametersEnd :
	public CompilerExpression
{
public:
	CompilerFunctionParametersEnd();
};

#endif