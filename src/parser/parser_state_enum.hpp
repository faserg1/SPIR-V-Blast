#ifndef PARSER_STATE_ENUM
#define PARSER_STATE_ENUM

enum class EParserState
{
	Global,
	GlobalType,
	GlobalNameDeclaration,
	FunctionParametersStart,
	FunctionParameterType,
	FunctionParameterName,
	FunctionParametersSeparator,
	FunctionParametersEnd,
	FunctionBodyStart,
	FunctionBodyEnd,
	FunctionLocal,
};

#endif