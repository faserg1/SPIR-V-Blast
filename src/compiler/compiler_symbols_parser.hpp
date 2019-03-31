#ifndef COMPILER_SYMBOLS_PARSER
#define COMPILER_SYMBOLS_PARSER

#include "compiler_parser.hpp"
#include "compiler_symbols.hpp"

class BlastSymbolsParser :
	public CommonParser
{
public:
	
protected:
	BlastSymbolsParser() = default;
private:
};

class BlastNameParser :
	public BlastSymbolsParser
{
public:
	bool tryVisit(const std::string &expression) override;
	bool next() override;
	std::shared_ptr<CompilerNode> end(EParserState state) override;
private:
	std::string name_;
};

#endif