#ifndef COMPILER_STATE_MACHINE
#define COMPILER_STATE_MACHINE

#include <memory>
#include <string>

#include "compiler_states.hpp"
#include "compiler_nodes.hpp"

class CommonParser;

class ParserStateMachine
{
public:
	ParserStateMachine();
	void feed(const std::string &expression);
	std::vector<std::shared_ptr<CompilerNode>> end();
private:
	const std::vector<ParserState> states_;
	std::unique_ptr<ParserState> currentState_;
	std::shared_ptr<CommonParser> currentParser_;

	std::vector<std::shared_ptr<CompilerNode>> nodes_;
};

#endif