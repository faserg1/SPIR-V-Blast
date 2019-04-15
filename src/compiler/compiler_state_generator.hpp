#ifndef COMPILER_STATE_GENERATOR
#define COMPILER_STATE_GENERATOR

#include <vector>
#include <memory>

class CompilerState;

std::vector<std::shared_ptr<CompilerState>> generateCompilerStates();

#endif