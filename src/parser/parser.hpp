#ifndef PARSER
#define PARSER

#include <vector>
#include <string>
#include <memory>

#include "../shader_preprocessed_info.hpp"
#include "../shader.hpp"

class ParserNode;

struct LiteralDebugInfo
{
	std::string literal;
	DebugRowInfo rowInfo;
};

class Parser
{
public:
	std::vector<std::shared_ptr<ParserNode>> parse(const ShaderPreprocessedInfo &preprocessedInfo);
private:
	std::vector<LiteralDebugInfo> splitByLiterals(std::string text, std::vector<DebugRowInfo> debugRowsInfo);
	std::vector<std::shared_ptr<ParserNode>> getNodes(std::vector<LiteralDebugInfo> literals);
};

#endif // PARSER
