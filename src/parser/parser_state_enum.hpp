#ifndef PARSER_STATE_ENUM
#define PARSER_STATE_ENUM

enum class EParserState
{
	Global,
	GlobalType,
	GlobalName,
	FunctionParametersStart,
	FunctionParameterType,
	FunctionParameterName,
	FunctionParametersSeparator,
	FunctionParametersEnd,
	FunctionBodyStart,
	FunctionBodyEnd,
	FunctionLocal, //? Forgot about this. Mb use as jump state? or just delete it
};

#endif