#ifndef COMPILER_COMMON
#define COMPILER_COMMON

#include "compiler_data.hpp"
#include "compiler_context.hpp"

class ShaderPreprocessedInfo;
class AbstractSyntaxTreeContainer;

class CompilerCommon
{
public:
	std::vector<SpirVOp> compile(std::shared_ptr<AbstractSyntaxTreeContainer> container, const ShaderPreprocessedInfo &ppInfo);
private:
	Id compileType(const Type &type);
	Id compileType(const FunctionType &type);
	Id compileType(const TypeInner &type);
	void applyStorageClass(Type &type, spv::StorageClass storageClass);
	void applyStorageClass(BaseVariable &type, spv::StorageClass storageClass);

	void compileMemoryModel(spv::MemoryModel memModel, spv::AddressingModel addrModel);
	void compileCapability(spv::Capability cap);

	void compileGlobalVariable(GlobalVariable &var);
	void compileStruct(const Struct &userStruct);
	void compileFunction(const Function &func);

	void decorate(const Id &id, spv::Decoration dec, std::vector<AttributeParam> params);
	void decorateMember(const Id &id, uint32_t memberPosition, spv::Decoration dec, std::vector<AttributeParam> params);
	void writeDecorationParams(SpirVOp &op, std::vector<AttributeParam> params);

	std::vector<SpirVOp> collectResult();

	SpirVOp debugOp(const Id &id);
	SpirVOp debugMemberOp(const Id &id, uint32_t memberPosition, std::string debugMemberName);
	OpParam paramId(const Id &id);
	OpParam paramInt(int64_t i, uint8_t size);
	OpParam paramUint(uint64_t u, uint8_t size);
	OpParam paramString(std::string str);
private:
	CompilerContext ctx_;
};

#endif