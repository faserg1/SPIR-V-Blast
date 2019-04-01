#ifndef PARSER
#define PARSER

#include <vector>
#include <string>
#include <memory>

#include "../shader_preprocessed_info.hpp"
#include "../shader.hpp"

class ParserNode;

class Parser
{
public:
	std::vector<std::shared_ptr<ParserNode>> parse(const ShaderPreprocessedInfo &preprocessedInfo);
private:
	std::vector<std::string> splitByLiterals(std::string text);
	std::vector<std::shared_ptr<ParserNode>> getNodes(std::vector<std::string> literals);
};

#endif // PARSER
