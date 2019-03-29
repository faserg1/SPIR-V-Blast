#include "compiler_states.hpp"
#include "compiler_parsers.hpp"

CompilerState::CompilerState(ECompilerState state, std::vector<std::shared_ptr<CompilerParser>> compilers, std::vector<ECompilerState> nextStates) :
	state_(state), compilers_(compilers), nextAvailableStates_(nextStates)
{

}

ECompilerState CompilerState::getState() const
{
	return state_;
}

std::vector<std::shared_ptr<CompilerParser>> CompilerState::getCompilers() const
{
	return compilers_;
}

std::vector<ECompilerState> CompilerState::getNextAvailableStates() const
{
	return nextAvailableStates_;
}

std::vector<CompilerState> generateStates()
{
	auto basicTypeParser = std::make_shared<CompilerBasicTypeParser>();
	auto nameParser = std::make_shared<CompilerNameParser>();
	auto expressionEndParser = std::make_shared<CompilerExpressionEndParser>();
	auto functionParamtersStart = std::make_shared<CompilerFunctionParametersStartParser>();
	auto functionParamtersSeparator = std::make_shared<CompilerFunctionParametersSeparatorParser>();
	auto functionParamtersEnd = std::make_shared<CompilerFunctionParametersEndParser>();
	return std::vector<CompilerState>
	{
		{ ECompilerState::GlobalState, { expressionEndParser }, { ECompilerState::GlobalTypeState }},
		{ ECompilerState::GlobalTypeState, { basicTypeParser }, {ECompilerState::GlobalNameState} },
		{ ECompilerState::GlobalNameState, { nameParser },
			{ECompilerState::GlobalState, ECompilerState::GlobalFunctionParametersStart}
		},
		{ ECompilerState::GlobalFunctionParametersStart, {functionParamtersStart},
			{ ECompilerState::GlobalFunctionParameterType, ECompilerState::GlobalFunctionParametersEnd}
		},
		{ ECompilerState::GlobalFunctionParameterType, { basicTypeParser },
			{ ECompilerState::GlobalFunctionParameterName, ECompilerState::GlobalFunctionParametersSeparator, ECompilerState::GlobalFunctionParametersEnd }
		},
		{ ECompilerState::GlobalFunctionParameterName, {nameParser}, 
			{ ECompilerState::GlobalFunctionParametersSeparator, ECompilerState::GlobalFunctionParametersEnd }
		},
		{ ECompilerState::GlobalFunctionParametersSeparator, {functionParamtersSeparator}, {ECompilerState::GlobalFunctionParameterType} },
		{ ECompilerState::GlobalFunctionParametersEnd, {functionParamtersEnd},
			{ ECompilerState::GlobalState }
		},
	};
}