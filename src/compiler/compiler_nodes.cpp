#include "compiler_nodes.hpp"

CompilerNode::CompilerNode(ECompilerNodeType type, std::any value, EParserState state) :
	nodeType_(type), value_(value), state_(state)
{
}

EParserState CompilerNode::getCompilerState() const
{
	return state_;
}
