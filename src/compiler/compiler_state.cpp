#include "compiler_state.hpp"
#include <stdexcept>

ECompilerState CompilerState::getState() const
{
	return state_;
}

ECompilerState CompilerState::getNextState(std::shared_ptr<ParserNode> node) const
{
	throw std::logic_error("Unable to find compiler state!");
}

bool CompilerState::canCompile() const
{
	return false;
}

bool CompilerState::isComplete() const
{
	return false;
}

std::shared_ptr<CompilerNode> CompilerState::compile(std::vector<std::shared_ptr<ParserNode>> parserNodes)
{
	return {};
}

CompilerState::CompilerState(ECompilerState state) :
	state_(state)
{
}
