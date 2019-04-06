#include "compiler_node.hpp"

ECompilerNodeType CompilerNode::getType() const
{
	return nodeType_;
}

CompilerNode::CompilerNode(ECompilerNodeType nodeType) :
	nodeType_(nodeType)
{
}
