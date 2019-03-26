#ifndef COMPILER_NODES
#define COMPILER_NODES

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

#endif // COMPILER_NODES