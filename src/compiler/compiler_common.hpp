#ifndef COMPILER_COMMON
#define COMPILER_COMMON

#include <memory>
#include "compiler_data.hpp"
#include "compiler_context.hpp"

class ShaderPreprocessedInfo;
class AbstractSyntaxTreeContainer;
class CompilerBodyCompilationContext;

class CompilerCommon
{
public:
	std::vector<SpirVOp> compile(std::shared_ptr<AbstractSyntaxTreeContainer> container, const ShaderPreprocessedInfo &ppInfo);
private:
	Id compileType(const Type &type);
	Id compileType(const FunctionType &type);
	Id compileType(const TypeInner &type);

	void applyStorageClass(TypeInner &type, spv::StorageClass storageClass);
	void applyStorageClass(BaseVariable &type, spv::StorageClass storageClass);

	void compileStorageClass(GlobalVariable &var);
	void compileDecorations(const Id &id, GlobalVariable &var);
	void compileDecorations(const Id &structId, const Struct &userStruct);
	void compileDecorations(const Id &structId, uint32_t memberPosition, const StructMember &member);

	void compileMemoryModel(spv::MemoryModel memModel, spv::AddressingModel addrModel);
	void compileCapability(spv::Capability cap);

	void compileGlobalVariable(GlobalVariable &var);
	void compileStruct(const Struct &userStruct);
	void compileFunction(const Function &func);
	std::vector<SpirVOp> compileFunctionBody(const Function &func, const Id &funcId);
	std::vector<SpirVOp> compileExpression(const Expression &expression, std::shared_ptr<CompilerBodyCompilationContext> parentContext);

	void compileConstExpression(const Expression &expression, Literal &l);
	void compileConstExpression(const Expression &expression, Identifier &i);
	Literal castToType(const Type &t, const Literal &l);
	void decorate(const Id &id, spv::Decoration dec, std::vector<AttributeParam> params);
	void decorateMember(const Id &id, uint32_t memberPosition, spv::Decoration dec, std::vector<AttributeParam> params);
	void writeDecorationParams(SpirVOp &op, std::vector<AttributeParam> params);

	std::vector<SpirVOp> collectResult();
private:
	CompilerContext ctx_;
	std::map<std::string, Id> structIds_;
	std::map<Id, Struct> structs_;
};

#endif