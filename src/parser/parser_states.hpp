#ifndef PARSER_STATES
#define PARSER_STATES

#include <memory>
#include <vector>

class CommonParser;

/*
TODO: [OOKAMI] States

state enum

state class = name, states available next, compiler parser

global state
global type state
global name state
func body state
local type state
etc..
*/

#include "parser_state_enum.hpp"

class ParserState
{
public:
	ParserState(EParserState state, std::vector<std::shared_ptr<CommonParser>> parsers, std::vector<EParserState> nextStates);

	EParserState getState() const;
	std::vector<std::shared_ptr<CommonParser>> getParsers() const;
	std::vector<EParserState> getNextAvailableStates() const;
private:
	const EParserState state_;
	const std::vector<std::shared_ptr<CommonParser>> parsers_;
	const std::vector<EParserState> nextAvailableStates_;
};

std::vector<ParserState> generateStates();

#endif