#ifndef COMPILER_STATE_MACHINE
#define COMPILER_STATE_MACHINE

#include <memory>
#include <string>

#include "compiler_states.hpp"
#include "compiler_nodes.hpp"

class CompilerParser;

class CompilerStateMachine
{
public:
	CompilerStateMachine();
	void feed(const std::string &expression);
	std::vector<std::shared_ptr<CompilerNode>> end();
private:
	const std::vector<CompilerState> states_;
	std::unique_ptr<CompilerState> currentState_;
	std::shared_ptr<CompilerParser> currentParser_;

	std::vector<std::shared_ptr<CompilerNode>> nodes_;
};

#endif