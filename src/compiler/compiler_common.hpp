#ifndef COMPILER_COMMON
#define COMPILER_COMMON

#include "compiler_data.hpp"
#include "compiler_context.hpp"

class ShaderPreprocessedInfo;
class AbstractSyntaxTreeContainer;

class CompilerCommon
{
public:
	void compile(std::shared_ptr<AbstractSyntaxTreeContainer> container, const ShaderPreprocessedInfo &ppInfo);
private:
	void compileType(const Type &type);
	void compileGlobalVariable(const GlobalVariable &var);
	void compileFunction(const Function &func);
private:
	CompilerContext ctx_;
};

#endif