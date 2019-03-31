#include "compiler_nodes.hpp"

CompilerNode::CompilerNode(ECompilerNodeType type, std::any value, ECompilerState state) :
	nodeType_(type), value_(value), state_(state)
{
}

ECompilerState CompilerNode::getCompilerState() const
{
	return state_;
}
