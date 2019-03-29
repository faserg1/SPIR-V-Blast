#ifndef COMPILER_STATE_ENUM
#define COMPILER_STATE_ENUM

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

#endif