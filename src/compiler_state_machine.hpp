#ifndef COMPILER_STATE_MACHINE
#define COMPILER_STATE_MACHINE

#include <memory>

class CompilerParser;

class CompilerStateMachine
{
public:
	std::shared_ptr<CompilerParser> getNextParser();
};

#endif