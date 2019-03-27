#include "compiler_states.hpp"
#include "compiler_parsers.hpp"

CompilerState::CompilerState(ECompilerState state, std::vector<std::shared_ptr<CompilerParser>> compilers, std::vector<ECompilerState> nextStates) :
	state_(state), compilers_(compilers), nextAvailableStates_(nextStates)
{

}

ECompilerState CompilerState::getState() const
{
	return state_;
}

std::vector<std::shared_ptr<CompilerParser>> CompilerState::getCompilers() const
{
	return compilers_;
}

std::vector<ECompilerState> CompilerState::getNextAvailableStates() const
{
	return nextAvailableStates_;
}

std::vector<CompilerState> generateStates()
{
	auto basicTypeParser = std::make_shared<CompilerBasicTypeParser>();
	return std::vector<CompilerState>
	{
		{ECompilerState::GlobalState, {}, {ECompilerState::GlobalTypeState}},
		{ECompilerState::GlobalTypeState, { basicTypeParser }, {}}
	};
}
