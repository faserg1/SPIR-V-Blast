#ifndef COMPILER_STATE
#define COMPILER_STATE

#include <memory>
#include <vector>
#include "compiler_state_enum.hpp"
#include "../parser/parser_node_type_enum.hpp"

class ParserNode;
class CompilerNode;

class CompilerState :
	public std::enable_shared_from_this<CompilerState>
{
public:
	ECompilerState getState() const;
	virtual ECompilerState getNextState(std::shared_ptr<ParserNode> node) const;
	virtual bool canCompile() const;
	virtual bool isComplete() const;
	virtual std::shared_ptr<CompilerNode> compile(std::vector<std::shared_ptr<ParserNode>> parserNodes);
protected:
	CompilerState(ECompilerState state);
private:
	const ECompilerState state_;
};

#endif