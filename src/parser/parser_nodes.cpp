#include "parser_nodes.hpp"

ParserNode::ParserNode(EParserNodeType type, std::any value, EParserState state) :
	nodeType_(type), value_(value), state_(state)
{
}

EParserState ParserNode::getParserState() const
{
	return state_;
}
