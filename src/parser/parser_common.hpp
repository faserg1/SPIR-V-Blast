#ifndef PARSER_COMMON
#define PARSER_COMMON

#include <memory>

class ShaderPreprocessedInfo;
class AbstractSyntaxTreeContainer;

class ParserCommon
{
public:
	std::shared_ptr<AbstractSyntaxTreeContainer> parse(const ShaderPreprocessedInfo &preprocessedInfo);
};

#endif