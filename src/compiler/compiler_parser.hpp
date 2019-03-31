#ifndef COMPILER_PARSER
#define COMPILER_PARSER

#include <memory>
#include <string>
#include "compiler_state_enum.hpp"

class CompilerNode;

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
	virtual std::shared_ptr<CompilerNode> end(EParserState state) = 0;
protected:
	CommonParser() = default;
};

#endif