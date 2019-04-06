#include "compiler.hpp"

void Compiler::compile(ShaderPreprocessedInfo preprocessedInfo, std::vector<std::shared_ptr<ParserNode>> nodes)
{
	
}

//TODO: Search the control parser nodes: function parameter start, expression end, function body start/end
//compile them in compiler nodes, but also save the function nodes in store (if there a prototype - it must have a body then)