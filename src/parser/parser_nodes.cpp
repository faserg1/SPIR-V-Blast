#include "parser_nodes.hpp"

ParserNode::ParserNode(EParserNodeType type, std::any value, EParserState state) :
	nodeType_(type), value_(value), state_(state)
{
}

EParserNodeType ParserNode::getNodeType() const
{
	return nodeType_;
}

EParserState ParserNode::getParserState() const
{
	return state_;
}
