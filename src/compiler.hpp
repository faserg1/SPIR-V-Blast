#ifndef COMPILER
#define COMPILER

#include "shader_preprocessed_info.hpp"
#include "shader.hpp"

class Compiler
{
public:
	Shader compile(const ShaderPreprocessedInfo &preprocessedInfo);
private:
	
};

#endif // COMPILER
