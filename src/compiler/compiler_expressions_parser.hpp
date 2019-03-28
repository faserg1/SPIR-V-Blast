#ifndef COMPILER_EXPRESSIONS_PARSER
#define COMPILER_EXPRESSIONS_PARSER

#include "compiler_parser.hpp"
#include "compiler_expressions.hpp"

class CompilerExpressionParser :
	public CompilerParser
{
protected:
	CompilerExpressionParser() = default;
};

class CompilerExpressionEndParser :
	public CompilerExpressionParser
{
public:
	CompilerExpressionEndParser() = default;

	bool tryVisit(const std::string &expression) override;
	bool next() override;
	std::shared_ptr<CompilerNode> end() override;
};

#endif