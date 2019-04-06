#ifndef COMPILER_NODE
#define COMPILER_NODE

#include <memory>
#include "compiler_node_type_enum.hpp"

class CompilerNode :
	public std::enable_shared_from_this<CompilerNode>
{
public:
	ECompilerNodeType getType() const;
protected:
	CompilerNode(ECompilerNodeType nodeType);
private:
	ECompilerNodeType nodeType_;
};

#endif // COMPILER_NODE