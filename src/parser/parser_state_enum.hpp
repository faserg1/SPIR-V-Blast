#ifndef PARSER_STATE_ENUM
#define PARSER_STATE_ENUM

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