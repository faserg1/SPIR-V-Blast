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
	Id compileType(const Type &type);
	Id compileType(const TypeInner &type);
	void compileGlobalVariable(const GlobalVariable &var);
	void compileFunction(const Function &func);
private:
	SpirVOp debugOp(const Id &id);
	OpParam paramId(const Id &id);
	OpParam paramInt(int64_t i);
	OpParam paramUint(uint64_t u);
	OpParam paramString(std::string str);
private:
	CompilerContext ctx_;
};

#endif