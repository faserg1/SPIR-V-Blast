#include "parser_state_machine.hpp"
#include "common_parser.hpp"
#include "parser_states_generator.hpp"
#include <algorithm>
#include <stdexcept>

static std::shared_ptr<IParserState> findState(const std::vector<std::shared_ptr<IParserState>> &states, EParserState state);
static std::shared_ptr<IParserState> findGlobalState(const std::vector<std::shared_ptr<IParserState>> &states);

ParserStateMachine::ParserStateMachine() :
	states_(generateStates()), currentState_(findGlobalState(states_))
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
	if (jumpState())
		return;
	auto nextAvailableStates = currentState_->getNextAvailableStates();
	std::vector<std::shared_ptr<IParserState>> nextStates;
	std::copy_if(states_.begin(), states_.end(), std::back_inserter(nextStates), [&nextAvailableStates](std::shared_ptr<IParserState> parserState) -> bool
	{
		const auto state = parserState->getState();
		const auto validState = std::any_of(nextAvailableStates.begin(), nextAvailableStates.end(), [state](EParserState availableState) -> bool {return availableState == state; });
		if (!validState)
			return false;
		if (!parserState->canActivate())
			return false;
		return true;
	});
	for (auto &state : nextStates)
	{
		auto parsers = state->getParsers();
		for (auto parser : parsers)
		{
			if (parser->tryVisit(expression))
			{
				currentParser_ = parser;
				break;
			}
		}
		if (currentParser_)
		{
			currentState_ = state;
			currentState_->activate();
			break;
		}
	}
	if (currentParser_ && currentState_)
		next();
	else
		throw std::runtime_error("Unexpected \"" + expression + "\".");
}

std::vector<std::shared_ptr<ParserNode>> ParserStateMachine::end()
{
	jumpState();
	if (currentState_->getState() != EParserState::Global)
		throw std::runtime_error("Unexpected end of shader!");
	return nodes_;
}

bool ParserStateMachine::jumpState()
{
	auto nextJumpState = currentState_->getNextJumpState();
	if (nextJumpState)
	{
		currentState_ = findState(states_, *nextJumpState);
		return true;
	}
	return false;
}

std::shared_ptr<IParserState> findState(const std::vector<std::shared_ptr<IParserState>> &states, EParserState state)
{
	return *std::find_if(states.begin(), states.end(), [state](std::shared_ptr<IParserState> stateObject) -> bool
	{
		return stateObject->getState() == state;
	});
}

std::shared_ptr<IParserState> findGlobalState(const std::vector<std::shared_ptr<IParserState>> &states)
{
	return findState(states, EParserState::Global);
}