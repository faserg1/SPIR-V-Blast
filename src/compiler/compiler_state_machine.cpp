#include "compiler_state_machine.hpp"
#include "compiler_state.hpp"
#include "compiler_state_generator.hpp"
#include <algorithm>

CompilerStateMachine::CompilerStateMachine()
{
	states_ = generateCompilerStates();
}

bool CompilerStateMachine::feed(std::shared_ptr<ParserNode> parserNode)
{
	if (!currentState_)
		setStateTo(ECompilerState::None);
	const auto nextState = currentState_->getNextState(parserNode);
	setStateTo(nextState);
	if (currentState_->isComplete() && currentState_->canCompile())
	{
		compiledNode_ = currentState_->compile(std::move(tempNodes_));
		setStateTo(ECompilerState::None);
		return true;
	}
	return false;
}

std::shared_ptr<CompilerNode> CompilerStateMachine::getNode()
{
	return compiledNode_;
}

void CompilerStateMachine::compileTempNodes(ECompilerNodeType resultType)
{
}

void CompilerStateMachine::setStateTo(ECompilerState eState)
{
	currentState_ = *std::find_if(states_.begin(), states_.end(), [eState](std::shared_ptr<CompilerState> state) -> bool
	{
		return state->getState() == eState;
	});
}
