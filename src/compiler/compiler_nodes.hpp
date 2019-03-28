#ifndef COMPILER_NODES
#define COMPILER_NODES

#include <memory>
#include <any>

enum class ECompilerNodeType
{
	/// Type node
	Type,
	/// Name of function or variable
	Name,
	/// String, symbol or number literal
	Literal,
	/// Compiler expression
	Expression,
};

class CompilerNode :
	public std::enable_shared_from_this<CompilerNode>
{
public:
	CompilerNode(ECompilerNodeType type, std::any value);
	template <class T>
	T getValue()
	{
		return std::any_cast<T>(value_);
	}
private:
	const ECompilerNodeType nodeType_;
	const std::any value_;
};

#endif // COMPILER_NODES