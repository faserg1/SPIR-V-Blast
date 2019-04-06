#ifndef COMPILER_STATE_GENERATOR
#define COMPILER_STATE_GENERATOR

#include <vector>
#include <memory>

class CompilerState;

class CompilerStateGenerator
{
public:
	CompilerStateGenerator() = default;
	std::vector<std::shared_ptr<CompilerState>> generateStates();
};

#endif