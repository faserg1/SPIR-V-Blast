#ifndef COMPILER_STATES
#define COMPILER_STATES

#include <stdexcept>
#include "compiler_state.hpp"
#include "../parser/parser_nodes.hpp"

class CompilerStateNone :
	public CompilerState
{
public:
	CompilerStateNone();
	ECompilerState getNextState(std::shared_ptr<ParserNode> node) const override;
};

class CompilerStateTypeDelaration :
	public CompilerState
{
public:
	CompilerStateTypeDelaration();
	ECompilerState getNextState(std::shared_ptr<ParserNode> node) const override;
};

class CompilerStateNameDelaration :
	public CompilerState
{
public:
	CompilerStateNameDelaration();
	ECompilerState getNextState(std::shared_ptr<ParserNode> node) const override;
};

class CompilerStateFunctionDelaration :
	public CompilerState
{
public:
	CompilerStateFunctionDelaration();
	ECompilerState getNextState(std::shared_ptr<ParserNode> node) const override;
};

#endif