#ifndef COMPILER_NODES
#define COMPILER_NODES

#include <memory>
#include <any>

#include "compiler_node_type_enum.hpp"
#include "compiler_state_enum.hpp"

class CompilerNode :
	public std::enable_shared_from_this<CompilerNode>
{
public:
	CompilerNode(ECompilerNodeType type, std::any value, ECompilerState state);
	template <class T>
	T getValue() const
	{
		return std::any_cast<T>(value_);
	}
	ECompilerState getCompilerState() const;
private:
	const ECompilerNodeType nodeType_;
	const std::any value_;
	const ECompilerState state_;
};

#endif // COMPILER_NODES