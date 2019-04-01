#ifndef COMMON_PARSER
#define COMMON_PARSER

#include <memory>
#include <string>
#include "parser_state_enum.hpp"

class ParserNode;

class CommonParser :
	public std::enable_shared_from_this<CommonParser>
{
public:
	virtual ~CommonParser() = default;

	/*
	@return true if valid expression, false if invalid or stop (stop after first capture)
	*/
	virtual bool tryVisit(const std::string &expression) = 0;
	/*
	@return true if can receive more data, false if stop
	*/
	virtual bool next() = 0;
	/*
	End capture. Get compiler node
	*/
	virtual std::shared_ptr<ParserNode> end(EParserState state) = 0;
protected:
	CommonParser() = default;
};

#endif