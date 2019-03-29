#ifndef COMPILER_STATES
#define COMPILER_STATES

#include <memory>
#include <vector>

class CompilerParser;

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

enum class ECompilerState
{
	GlobalState,
	GlobalTypeState,
	GlobalNameState,
	GlobalFunctionParametersStart,
	GlobalFunctionParameterType,
	GlobalFunctionParameterName,
	GlobalFunctionParametersSeparator,
	GlobalFunctionParametersEnd,
};

class CompilerState
{
public:
	CompilerState(ECompilerState state, std::vector<std::shared_ptr<CompilerParser>> compilers, std::vector<ECompilerState> nextStates);

	ECompilerState getState() const;
	std::vector<std::shared_ptr<CompilerParser>> getCompilers() const;
	std::vector<ECompilerState> getNextAvailableStates() const;
private:
	const ECompilerState state_;
	const std::vector<std::shared_ptr<CompilerParser>> compilers_;
	const std::vector<ECompilerState> nextAvailableStates_;
};

std::vector<CompilerState> generateStates();

#endif