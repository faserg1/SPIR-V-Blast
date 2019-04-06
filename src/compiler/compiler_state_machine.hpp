#ifndef COMPILER_STATE_MACHINE
#define COMPILER_STATE_MACHINE

#include <memory>
#include <vector>

#include "compiler_node_type_enum.hpp"

class ParserNode;
class CompilerNode;

class CompilerStateMachine
{
public:
	bool feed(std::shared_ptr<ParserNode> parserNode);
	std::shared_ptr<CompilerNode> getNode();
private:
	std::vector<std::shared_ptr<ParserNode>> tempNodes_;
	std::shared_ptr<CompilerNode> compiledNode_;
private:
	void compileTempNodes(ECompilerNodeType resultType);
};

#endif