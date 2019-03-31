#ifndef PARSER
#define PARSER

#include <vector>
#include <string>
#include <memory>

#include "../shader_preprocessed_info.hpp"
#include "../shader.hpp"

class CompilerNode;

class Parser
{
public:
	Shader parse(const ShaderPreprocessedInfo &preprocessedInfo);
private:
	std::vector<std::string> splitByLiterals(std::string text);
	std::vector<std::shared_ptr<CompilerNode>> getNodes(std::vector<std::string> literals);
};

#endif // PARSER
