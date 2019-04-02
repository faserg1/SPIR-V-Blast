#include "parser_states_generator.hpp"
#include "parser_states.hpp"
#include "parsers.hpp"

#include <functional>
#include <algorithm>

std::vector<std::shared_ptr<IParserState>> generateStates()
{
	std::vector<std::shared_ptr<IParserState>> states;

	auto basicTypeParser = std::make_shared<BasicBlastTypeParser>();
	auto nameParser = std::make_shared<BlastNameParser>();
	auto expressionEndParser = std::make_shared<BlastExpressionEndParser>();

	auto functionParamtersStart = std::make_shared<BlastFunctionParametersStartParser>();
	auto functionParamtersSeparator = std::make_shared<BlastFunctionParametersSeparatorParser>();
	auto functionParamtersEnd = std::make_shared<BlastFunctionParametersEndParser>();

	auto expressionBodyStart = std::make_shared<BlastExpressionBodyStartParser>();
	auto expressionBodyEnd = std::make_shared<BlastExpressionBodyEndParser>();

	auto simpleStates = std::move(std::vector<SimpleParserState>
	{
		{ EParserState::Global, { expressionEndParser },
		{ EParserState::GlobalType }
		},
		{ EParserState::GlobalType, { basicTypeParser },
			{EParserState::GlobalName}
		},
		{ EParserState::GlobalName, { nameParser },
			{EParserState::Global, EParserState::FunctionParametersStart}
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
			{ EParserState::Global, EParserState::FunctionBodyStart }
		},
		{ EParserState::FunctionBodyStart, {expressionBodyStart},
			{EParserState::FunctionBodyEnd}
		},
		{ EParserState::FunctionBodyEnd, {expressionBodyEnd},
			{EParserState::Global}
		}
	});

	std::transform(simpleStates.begin(), simpleStates.end(), std::back_inserter(states), [](SimpleParserState &simpleState) -> std::shared_ptr<IParserState>
	{
		return std::make_shared<SimpleParserState>(simpleState);
	});

	return std::move(states);
}