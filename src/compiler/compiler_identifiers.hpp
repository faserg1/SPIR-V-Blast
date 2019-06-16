#ifndef COMPILER_IDENTIFIERS
#define COMPILER_IDENTIFIERS

#include <string>
#include <cstdint>
#include <map>
#include "compiler_data.hpp"
#include "../gen/blast_tokens.hpp"

class CompilerIdentifiers
{
public:
	CompilerIdentifiers();

	bool hasType(const TypeInner &t) const;
	bool hasType(const StructureType &t) const;
	bool hasType(const FunctionType &t) const;
	bool hasConstant(const Type &t, Literal &value) const;
	Id getTypeId(const TypeInner &t);
	Id getTypeId(const StructureType &t);
	Id getTypeId(const FunctionType &t);
	Id getConstantId(const Type &t, Literal &value);
	Id getVariableId(const BaseVariable &var);
	Id getFunctionId(const Function &func);
private:
	uint32_t counter_;
	std::map<TypeInner, Id> types_;
	std::map<StructureType, Id> structureTypes_;
	std::map<FunctionType, Id> functionTypes_;
	std::map<ConstExpression, Id> constants_;
	std::map<uint64_t, Id> vars_;
	std::map<uint64_t, Id> functions_;
private:
	Id createId(const std::string &debugName);
	std::string toDebugName(const TypeInner &t);
	std::string toDebugName(const StructureType &t);
	std::string toDebugName(const FunctionType &t);
	std::string toDebugName(const ConstExpression &e);
};

#endif // COMPILER_IDENTIFIERS