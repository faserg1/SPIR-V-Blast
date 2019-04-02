#ifndef PARSER_STATE
#define PARSER_STATE

#include <memory>
#include <vector>
#include <optional>

#include "parser_state_enum.hpp"

class CommonParser;

class IParserState
{
public:
	virtual ~IParserState() = default;

	virtual bool canActivate() const = 0;
	virtual void activate() = 0;
	virtual EParserState getState() const = 0;
	virtual std::vector<std::shared_ptr<CommonParser>> getParsers() const = 0;
	virtual std::optional<EParserState> getNextJumpState() const = 0;
	virtual std::vector<EParserState> getNextAvailableStates() const = 0;
protected:
	IParserState() = default;
};

#endif