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

	auto expressionBodyStart = std::make_shared<BlastExpressionBodyStartParser>();
	auto expressionBodyEnd = std::make_shared<BlastExpressionBodyEndParser>();

	return std::move(std::vector<ParserState>
	{
		{ EParserState::GlobalState, { expressionEndParser },
			{ EParserState::GlobalTypeState }
		},
		{ EParserState::GlobalTypeState, { basicTypeParser },
			{EParserState::GlobalNameState}
		},
		{ EParserState::GlobalNameState, { nameParser },
			{EParserState::GlobalState, EParserState::FunctionParametersStart}
		},
		{ EParserState::FunctionParametersStart, {functionParamtersStart},
			{ EParserState::FunctionParameterType, EParserState::FunctionParametersEnd}
		},
		{ EParserState::FunctionParameterType, { basicTypeParser },
			{ EParserState::FunctionParameterName, EParserState::FunctionParametersSeparator, EParserState::FunctionParametersEnd }
		},
		{ EParserState::FunctionParameterName, {nameParser}, 
			{ EParserState::FunctionParametersSeparator, EParserState::FunctionParametersEnd }
		},
		{ EParserState::FunctionParametersSeparator, {functionParamtersSeparator},
			{EParserState::FunctionParameterType}
		},
		{ EParserState::FunctionParametersEnd, {functionParamtersEnd},
			{ EParserState::GlobalState, EParserState::FunctionBodyStart }
		},
		{ EParserState::FunctionBodyStart, {expressionBodyStart},
			{EParserState::FunctionBodyEnd}
		},
		{ EParserState::FunctionBodyEnd, {expressionBodyEnd},
			{EParserState::GlobalState}
		}
	});
}