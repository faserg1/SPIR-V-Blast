#ifndef PARSER_EXPRESSIONS_PARSER
#define PARSER_EXPRESSIONS_PARSER

#include "common_parser.hpp"
#include "parser_expressions.hpp"

class BlastExpressionParser :
	public CommonParser
{
protected:
	BlastExpressionParser() = default;
};

class BlastExpressionEndParser :
	public BlastExpressionParser
{
public:
	BlastExpressionEndParser() = default;

	bool tryVisit(const std::string &expression) override;
	bool next() override;
	std::shared_ptr<ParserNode> end(EParserState state) override;
};

class BlastFunctionParametersStartParser :
	public BlastExpressionParser
{
public:
	BlastFunctionParametersStartParser() = default;

	bool tryVisit(const std::string &expression) override;
	bool next() override;
	std::shared_ptr<ParserNode> end(EParserState state) override;
};

class BlastFunctionParametersSeparatorParser :
	public BlastExpressionParser
{
public:
	BlastFunctionParametersSeparatorParser() = default;

	bool tryVisit(const std::string &expression) override;
	bool next() override;
	std::shared_ptr<ParserNode> end(EParserState state) override;
};

class BlastFunctionParametersEndParser :
	public BlastExpressionParser
{
public:
	BlastFunctionParametersEndParser() = default;

	bool tryVisit(const std::string &expression) override;
	bool next() override;
	std::shared_ptr<ParserNode> end(EParserState state) override;
};

class BlastExpressionBodyStartParser :
	public BlastExpressionParser
{
public:
	BlastExpressionBodyStartParser() = default;

	bool tryVisit(const std::string &expression) override;
	bool next() override;
	std::shared_ptr<ParserNode> end(EParserState state) override;
};

class BlastExpressionBodyEndParser :
	public BlastExpressionParser
{
public:
	BlastExpressionBodyEndParser() = default;

	bool tryVisit(const std::string &expression) override;
	bool next() override;
	std::shared_ptr<ParserNode> end(EParserState state) override;
};

#endif