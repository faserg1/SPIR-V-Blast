#include "compiler_nodes.hpp"

CompilerNode::CompilerNode(ECompilerNodeType type, std::any value) :
	nodeType_(type), value_(value)
{
}
