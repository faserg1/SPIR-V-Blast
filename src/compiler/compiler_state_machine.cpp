#include "compiler_state_machine.hpp"
#include "compiler_parser.hpp"
#include <algorithm>
#include <stdexcept>

static const CompilerState &findGlobalState(const std::vector<CompilerState> &states);

CompilerStateMachine::CompilerStateMachine() :
	states_(generateStates()), currentState_(std::make_unique<CompilerState>(findGlobalState(states_)))
{
}

void CompilerStateMachine::feed(const std::string &expression)
{
	auto next = [this]()
	{
		if (!currentParser_->next())
		{
			nodes_.push_back(currentParser_->end(currentState_->getState()));
			currentParser_ = {};
		}
	};

	if (currentParser_)
	{
		if (currentParser_->tryVisit(expression))
		{
			next();
			return;
		}
		else
		{
			nodes_.push_back(currentParser_->end(currentState_->getState()));
			currentParser_ = {};
		}
	}
	auto nextAvailableStates = currentState_->getNextAvailableStates();
	std::vector<CompilerState> nextStates;
	std::copy_if(states_.begin(), states_.end(), std::back_inserter(nextStates), [&nextAvailableStates](const CompilerState &compilerState) -> bool
	{
		const auto state = compilerState.getState();
		return std::any_of(nextAvailableStates.begin(), nextAvailableStates.end(), [state](ECompilerState availableState) -> bool {return availableState == state; });
	});
	for (auto &state : nextStates)
	{
		auto compilers = state.getCompilers();
		for (auto compiler : compilers)
		{
			if (compiler->tryVisit(expression))
			{
				currentParser_ = compiler;
				break;
			}
		}
		if (currentParser_)
		{
			currentState_ = std::make_unique<CompilerState>(state);
			break;
		}
	}
	if (currentParser_ && currentState_)
		next();
	else
		throw std::runtime_error("Unexpected \"" + expression + "\".");
}

std::vector<std::shared_ptr<CompilerNode>> CompilerStateMachine::end()
{
	if (currentState_->getState() != ECompilerState::GlobalState)
		throw std::runtime_error("Unexpected end of shader!");
	return nodes_;
}

const CompilerState &findGlobalState(const std::vector<CompilerState> &states)
{
	return *std::find_if(states.begin(), states.end(), [](const CompilerState &state) -> bool
	{
		return state.getState() == ECompilerState::GlobalState;
	});
}