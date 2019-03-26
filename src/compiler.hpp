#ifndef COMPILER
#define COMPILER

#include <vector>
#include <string>
#include <memory>

#include "shader_preprocessed_info.hpp"
#include "shader.hpp"

class CompilerNode;

class Compiler
{
public:
	Shader compile(const ShaderPreprocessedInfo &preprocessedInfo);
private:
	std::vector<std::string> splitByLiterals(std::string text);
	std::vector<std::shared_ptr<CompilerNode>> getNodes(std::vector<std::string> literals);
};

#endif // COMPILER
