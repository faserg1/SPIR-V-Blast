#include "compiler_state_generator.hpp"
#include "compiler_states.hpp"
#include <type_traits>

template <class ...TCompilerStates>
std::vector<std::shared_ptr<CompilerState>> generateShared()
{
	static_assert((std::is_base_of_v<CompilerState, TCompilerStates> && ...), "Some classes are not base of CompilerState!");
	return
	{
		std::make_shared<TCompilerStates>()...
	};
}

std::vector<std::shared_ptr<CompilerState>> generateCompilerStates()
{
	return generateShared<
		CompilerStateNone,
		CompilerStateTypeDelaration,
		CompilerStateNameDelaration,
		CompilerStateFunctionDelaration
	>();
}
