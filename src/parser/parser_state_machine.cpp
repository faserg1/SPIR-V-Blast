#include "parser_state_machine.hpp"
#include "common_parser.hpp"
#include <algorithm>
#include <stdexcept>

static const ParserState &findGlobalState(const std::vector<ParserState> &states);

ParserStateMachine::ParserStateMachine() :
	states_(generateStates()), currentState_(std::make_unique<ParserState>(findGlobalState(states_)))
{
}

void ParserStateMachine::feed(const std::string &expression)
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
	std::vector<ParserState> nextStates;
	std::copy_if(states_.begin(), states_.end(), std::back_inserter(nextStates), [&nextAvailableStates](const ParserState &compilerState) -> bool
	{
		const auto state = compilerState.getState();
		return std::any_of(nextAvailableStates.begin(), nextAvailableStates.end(), [state](EParserState availableState) -> bool {return availableState == state; });
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
			currentState_ = std::make_unique<ParserState>(state);
			break;
		}
	}
	if (currentParser_ && currentState_)
		next();
	else
		throw std::runtime_error("Unexpected \"" + expression + "\".");
}

std::vector<std::shared_ptr<CompilerNode>> ParserStateMachine::end()
{
	if (currentState_->getState() != EParserState::GlobalState)
		throw std::runtime_error("Unexpected end of shader!");
	return nodes_;
}

const ParserState &findGlobalState(const std::vector<ParserState> &states)
{
	return *std::find_if(states.begin(), states.end(), [](const ParserState &state) -> bool
	{
		return state.getState() == EParserState::GlobalState;
	});
}