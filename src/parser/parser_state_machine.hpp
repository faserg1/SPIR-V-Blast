#ifndef PARSER_STATE_MACHINE
#define PARSER_STATE_MACHINE

#include <memory>
#include <string>

#include "parser_states.hpp"
#include "parser_nodes.hpp"

class CommonParser;

class ParserStateMachine
{
public:
	ParserStateMachine();
	void feed(const std::string &expression);
	std::vector<std::shared_ptr<ParserNode>> end();
private:
	const std::vector<ParserState> states_;
	std::unique_ptr<ParserState> currentState_;
	std::shared_ptr<CommonParser> currentParser_;

	std::vector<std::shared_ptr<ParserNode>> nodes_;
};

#endif