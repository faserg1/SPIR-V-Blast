#ifndef COMPILER
#define COMPILER

#include <vector>
#include <string>

#include "shader_preprocessed_info.hpp"
#include "shader.hpp"

class Compiler
{
public:
	Shader compile(const ShaderPreprocessedInfo &preprocessedInfo);
private:
	std::vector<std::string> splitByLiterals(std::string text);
};

#endif // COMPILER
