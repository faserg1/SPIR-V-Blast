#include "compiler_symbols_parser.hpp"
#include "compiler_symbols.hpp"
#include "compiler_nodes.hpp"
#include <regex>

bool CompilerNameParser::tryVisit(const std::string &expression)
{
	std::regex literalRegex("^(?![0-9])\\w+$", std::regex_constants::ECMAScript);
	std::sregex_iterator end;
	auto literalMatchIter = std::sregex_iterator(expression.begin(), expression.end(), literalRegex);
	if (literalMatchIter == end)
		return false;
	name_ = expression;
	return true;
}

bool CompilerNameParser::next()
{
	return false;
}

std::shared_ptr<CompilerNode> CompilerNameParser::end(ECompilerState state)
{
	auto nameSymbol = std::make_shared<CompilerSymbolName>(name_);
	return std::make_shared<CompilerNode>(ECompilerNodeType::Name, nameSymbol, state);
}
