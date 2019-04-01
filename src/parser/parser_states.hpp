#ifndef PARSER_STATES
#define PARSER_STATES

#include <memory>
#include <vector>
#include <optional>

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

#include "parser_state_enum.hpp"

class IParserState
{
public:
	virtual ~IParserState() = default;

	virtual void activate() = 0;
	virtual EParserState getState() const = 0;
	virtual std::vector<std::shared_ptr<CommonParser>> getParsers() const = 0;
	virtual std::optional<EParserState> getNextJumpState() const = 0;
	virtual std::vector<EParserState> getNextAvailableStates() const = 0;
protected:
	IParserState() = default;
};

class SimpleParserState :
	public virtual IParserState
{
public:
	SimpleParserState(EParserState state, std::vector<std::shared_ptr<CommonParser>> parsers, std::vector<EParserState> nextStates);

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

std::vector<std::shared_ptr<IParserState>> generateStates();

#endif