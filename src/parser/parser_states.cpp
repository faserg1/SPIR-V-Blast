#include "parser_states.hpp"
#include "parsers.hpp"

ParserState::ParserState(EParserState state, std::vector<std::shared_ptr<CommonParser>> parsers, std::vector<EParserState> nextStates) :
	state_(state), parsers_(parsers), nextAvailableStates_(nextStates)
{

}

EParserState ParserState::getState() const
{
	return state_;
}

std::vector<std::shared_ptr<CommonParser>> ParserState::getParsers() const
{
	return parsers_;
}

std::vector<EParserState> ParserState::getNextAvailableStates() const
{
	return nextAvailableStates_;
}

std::vector<ParserState> generateStates()
{
	auto basicTypeParser = std::make_shared<BasicBlastTypeParser>();
	auto nameParser = std::make_shared<BlastNameParser>();
	auto expressionEndParser = std::make_shared<BlastExpressionEndParser>();
	auto functionParamtersStart = std::make_shared<BlastFunctionParametersStartParser>();
	auto functionParamtersSeparator = std::make_shared<BlastFunctionParametersSeparatorParser>();
	auto functionParamtersEnd = std::make_shared<BlastFunctionParametersEndParser>();
	return std::move(std::vector<ParserState>
	{
		{ EParserState::GlobalState, { expressionEndParser }, { EParserState::GlobalTypeState }},
		{ EParserState::GlobalTypeState, { basicTypeParser }, {EParserState::GlobalNameState} },
		{ EParserState::GlobalNameState, { nameParser },
			{EParserState::GlobalState, EParserState::GlobalFunctionParametersStart}
		},
		{ EParserState::GlobalFunctionParametersStart, {functionParamtersStart},
			{ EParserState::GlobalFunctionParameterType, EParserState::GlobalFunctionParametersEnd}
		},
		{ EParserState::GlobalFunctionParameterType, { basicTypeParser },
			{ EParserState::GlobalFunctionParameterName, EParserState::GlobalFunctionParametersSeparator, EParserState::GlobalFunctionParametersEnd }
		},
		{ EParserState::GlobalFunctionParameterName, {nameParser}, 
			{ EParserState::GlobalFunctionParametersSeparator, EParserState::GlobalFunctionParametersEnd }
		},
		{ EParserState::GlobalFunctionParametersSeparator, {functionParamtersSeparator}, {EParserState::GlobalFunctionParameterType} },
		{ EParserState::GlobalFunctionParametersEnd, {functionParamtersEnd},
			{ EParserState::GlobalState }
		},
	});
}