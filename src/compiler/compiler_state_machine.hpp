#ifndef COMPILER_STATE_MACHINE
#define COMPILER_STATE_MACHINE

#include <memory>
#include <vector>

#include "compiler_state_enum.hpp"
#include "compiler_node_type_enum.hpp"

class ParserNode;
class CompilerNode;
class CompilerState;

class CompilerStateMachine
{
public:
	CompilerStateMachine();
	bool feed(std::shared_ptr<ParserNode> parserNode);
	std::shared_ptr<CompilerNode> getNode();
private:
	std::shared_ptr<CompilerState> currentState_;
	std::vector<std::shared_ptr<CompilerState>> states_;
	std::vector<std::shared_ptr<ParserNode>> tempNodes_;
	std::shared_ptr<CompilerNode> compiledNode_;
private:
	void compileTempNodes(ECompilerNodeType resultType);
	void setStateTo(ECompilerState eState);
};

#endif