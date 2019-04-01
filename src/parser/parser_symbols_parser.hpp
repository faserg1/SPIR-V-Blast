#ifndef PARSER_SYMBOLS_PARSER
#define PARSER_SYMBOLS_PARSER

#include "common_parser.hpp"
#include "parser_symbols.hpp"

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
	std::shared_ptr<ParserNode> end(EParserState state) override;
private:
	std::string name_;
};

#endif