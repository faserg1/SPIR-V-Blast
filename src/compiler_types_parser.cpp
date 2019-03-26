#include "compiler_types_parser.hpp"

bool CompilerTypeParser::isType(std::string expression) const
{
	// Base type
	if (expression == "void" ||
		expression == "bool" ||
		expression == "int" ||
		expression == "float")
		return true;
	return false;
}
