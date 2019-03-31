#ifndef COMPILER_NODES
#define COMPILER_NODES

#include <memory>
#include <any>

#include "compiler_node_type_enum.hpp"
#include "../parser/compiler_state_enum.hpp"

class CompilerNode :
	public std::enable_shared_from_this<CompilerNode>
{
public:
	CompilerNode(ECompilerNodeType type, std::any value, EParserState state);
	template <class T>
	T getValue() const
	{
		return std::any_cast<T>(value_);
	}
	EParserState getCompilerState() const;
private:
	const ECompilerNodeType nodeType_;
	const std::any value_;
	const EParserState state_;
};

#endif // COMPILER_NODES