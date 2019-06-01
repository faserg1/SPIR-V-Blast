#include "compiler_common.hpp"
#include "../gen/blast_tokens.hpp"

void CompilerCommon::compile(std::shared_ptr<AbstractSyntaxTreeContainer> container, const ShaderPreprocessedInfo &ppInfo)
{
	auto &globalVariables = container->getGlobalVariables();
	for (auto &globalVar : globalVariables)
		compileGlobalVariable(globalVar);
}

void CompilerCommon::compileType(const Type &type)
{
	
}

void CompilerCommon::compileGlobalVariable(const GlobalVariable &var)
{
	
}

void CompilerCommon::compileFunction(const Function &func)
{
	
}
