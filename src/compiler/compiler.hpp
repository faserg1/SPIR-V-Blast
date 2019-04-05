#ifndef COMPILER
#define COMPILER

#include <vector>
#include <memory>
#include <cstdint>
#include "../shader_preprocessed_info.hpp"
#include "../parser/parser_nodes.hpp"

class Compiler
{
public:
	void compile(ShaderPreprocessedInfo preprocessedInfo, std::vector<std::shared_ptr<ParserNode>> nodes);
};

#endif