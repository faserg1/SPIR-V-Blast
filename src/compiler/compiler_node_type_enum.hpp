#ifndef COMPILER_NODE_TYPE_ENUM
#define COMPILER_NODE_TYPE_ENUM

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

#endif // COMPILER_NODE_TYPE_ENUM