#ifndef PARSER_STATE_ENUM
#define PARSER_STATE_ENUM

enum class EParserState
{
	GlobalState,
	GlobalTypeState,
	GlobalNameState,
	FunctionParametersStart,
	FunctionParameterType,
	FunctionParameterName,
	FunctionParametersSeparator,
	FunctionParametersEnd,
	FunctionBodyStart,
	FunctionBodyEnd,
};

#endif