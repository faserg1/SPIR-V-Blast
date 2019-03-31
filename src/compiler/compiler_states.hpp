#ifndef COMPILER_STATES
#define COMPILER_STATES

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

#include "compiler_state_enum.hpp"

class ParserState
{
public:
	ParserState(EParserState state, std::vector<std::shared_ptr<CommonParser>> compilers, std::vector<EParserState> nextStates);

	EParserState getState() const;
	std::vector<std::shared_ptr<CommonParser>> getCompilers() const;
	std::vector<EParserState> getNextAvailableStates() const;
private:
	const EParserState state_;
	const std::vector<std::shared_ptr<CommonParser>> compilers_;
	const std::vector<EParserState> nextAvailableStates_;
};

std::vector<ParserState> generateStates();

#endif