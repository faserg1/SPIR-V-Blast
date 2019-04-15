#include "compiler.hpp"
#include "compiler_state_machine.hpp"

void Compiler::compile(ShaderPreprocessedInfo preprocessedInfo, std::vector<std::shared_ptr<ParserNode>> nodes)
{
	CompilerStateMachine stateMachine;
	for (auto node : nodes)
	{
		if (stateMachine.feed(node))
			nodes_.push_back(stateMachine.getNode());
	}
}

//TODO: Search the control parser nodes: function parameter start, expression end, function body start/end
//compile them in compiler nodes, but also save the function nodes in store (if there a prototype - it must have a body then)