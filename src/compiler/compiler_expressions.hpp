#ifndef COMPILER_EXPRESSIONS
#define COMPILER_EXPRESSIONS

enum class ECompilerExpressionType
{
	End,
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

#endif