#ifndef PARSER_ABSTRACT_SYNTAX_TREE_CONTAINER
#define PARSER_ABSTRACT_SYNTAX_TREE_CONTAINER

#include <vector>
#include "parser_types.hpp"

class AbstractSyntaxTreeContainer
{
public:
	virtual std::vector<struct Struct> getStructs() const = 0;
	virtual std::vector<struct GlobalVariable> getGlobalVariables() const = 0;
	virtual std::vector<struct Function> getFunctions() const = 0;
	virtual std::vector<struct Enum> getEnums() const = 0;
protected:
	AbstractSyntaxTreeContainer() = default;
	virtual ~AbstractSyntaxTreeContainer() = default;
};

#endif