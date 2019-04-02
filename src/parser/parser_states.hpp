#ifndef PARSER_STATES
#define PARSER_STATES

#include "parser_state.hpp"

class SimpleParserState :
	public virtual IParserState
{
public:
	SimpleParserState(EParserState state, std::vector<std::shared_ptr<CommonParser>> parsers, std::vector<EParserState> nextStates);

	bool canActivate() const override;
	void activate() override;
	EParserState getState() const override;
	std::vector<std::shared_ptr<CommonParser>> getParsers() const override;
	std::optional<EParserState> getNextJumpState() const override;
	std::vector<EParserState> getNextAvailableStates() const override;
private:
	const EParserState state_;
	const std::vector<std::shared_ptr<CommonParser>> parsers_;
	const std::vector<EParserState> nextAvailableStates_;
};

class ConditionalParserState :
	public virtual IParserState
{
public:
	ConditionalParserState(EParserState state);

	EParserState getState() const override;
private:
	const EParserState state_;
};

#endif