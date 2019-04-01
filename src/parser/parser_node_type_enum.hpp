#ifndef PARSER_NODE_TYPE_ENUM
#define PARSER_NODE_TYPE_ENUM

enum class EParserNodeType
{
	/// Type node
	Type,
	/// Name of function or variable
	Name,
	/// String, symbol or number literal
	Literal,
	/// End of expression, ...
	Expression,
};

#endif // PARSER_NODE_TYPE_ENUM