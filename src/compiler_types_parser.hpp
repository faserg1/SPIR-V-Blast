#ifndef COMPILER_TYPES_PARSER
#define COMPILER_TYPES_PARSER

#include <string>
#include "compiler_parser.hpp"

class CompilerTypeParser :
	public CompilerParser
{
public:
	CompilerTypeParser() = default;

	bool isType(std::string expression) const;
private:
};

#endif