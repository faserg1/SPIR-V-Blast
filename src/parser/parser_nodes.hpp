#ifndef PARSER_NODES
#define PARSER_NODES

#include <memory>
#include <any>

#include "parser_node_type_enum.hpp"
#include "parser_state_enum.hpp"

class ParserNode :
	public std::enable_shared_from_this<ParserNode>
{
public:
	ParserNode(EParserNodeType type, std::any value, EParserState state);
	template <class T>
	T getValue() const
	{
		return std::any_cast<T>(value_);
	}
	EParserState getParserState() const;
private:
	const EParserNodeType nodeType_;
	const std::any value_;
	const EParserState state_;
};

#endif // PARSER_NODES