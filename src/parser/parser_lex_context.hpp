#ifndef PARSER_LEX_CONTEXT
#define PARSER_LEX_CONTEXT

#include <string>
#include "parser_types.hpp"

class LexContext
{
public:
	virtual IdentifierType getIdentifierType(std::string name) const = 0;
protected:
	LexContext() = default;
	virtual ~LexContext() = default;
};

#endif