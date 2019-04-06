#include "compiler_state_machine.hpp"

bool CompilerStateMachine::feed(std::shared_ptr<ParserNode> parserNode)
{
	return false;
}

std::shared_ptr<CompilerNode> CompilerStateMachine::getNode()
{
	return compiledNode_;
}

void CompilerStateMachine::compileTempNodes(ECompilerNodeType resultType)
{
}
