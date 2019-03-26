#ifndef COMPILER_PARSER
#define COMPILER_PARSER

#include <memory>

enum class ECompilerNodeType
{
	TypeNode,
	LiteralNode,
};

class CompilerNode :
	public std::enable_shared_from_this<CompilerNode>
{
public:
	
};

class CompilerParser
{
public:
	virtual ~CompilerParser() = default;
protected:
	CompilerParser() = default;
};

#endif