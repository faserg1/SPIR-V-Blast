#ifndef COMPILER_STATE_ENUM
#define COMPILER_STATE_ENUM

enum class EParserState
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