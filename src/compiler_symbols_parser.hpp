#ifndef COMPILER_SYMBOLS_PARSER
#define COMPILER_SYMBOLS_PARSER

#include "compiler_parser.hpp"
#include "compiler_symbols.hpp"

class CompilerSymbolsParser :
	public CompilerParser
{
public:
	
protected:
	CompilerSymbolsParser() = default;
private:
};

class CompilerNameParser :
	public CompilerSymbolsParser
{
public:
	bool tryVisit(const std::string &expression) override;
	bool next() override;
	std::shared_ptr<CompilerNode> end() override;
private:
	std::string name_;
};

#endif