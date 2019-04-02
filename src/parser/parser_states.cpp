#include "parser_states.hpp"

SimpleParserState::SimpleParserState(EParserState state, std::vector<std::shared_ptr<CommonParser>> parsers, std::vector<EParserState> nextStates) :
	state_(state), parsers_(parsers), nextAvailableStates_(nextStates)
{

}

bool SimpleParserState::canActivate() const
{
	return true;
}

void SimpleParserState::activate()
{
}

EParserState SimpleParserState::getState() const
{
	return state_;
}

std::vector<std::shared_ptr<CommonParser>> SimpleParserState::getParsers() const
{
	return parsers_;
}

std::optional<EParserState> SimpleParserState::getNextJumpState() const
{
	return {};
}

std::vector<EParserState> SimpleParserState::getNextAvailableStates() const
{
	return nextAvailableStates_;
}

ConditionalParserState::ConditionalParserState(EParserState state) :
	state_(state)
{
}

EParserState ConditionalParserState::getState() const
{
	return state_;
}
