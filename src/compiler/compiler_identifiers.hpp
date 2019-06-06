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
	bool hasType(const FunctionType &t) const;
	Id getTypeId(const TypeInner &t);
	Id getTypeId(const FunctionType &t);
	Id getVariableId(const BaseVariable &var);
private:
	uint32_t counter_;
	std::map<TypeInner, Id> types_;
	std::map<FunctionType, Id> functionTypes_;
	std::map<uint64_t, Id> vars_;
private:
	Id createId(const std::string &debugName);
	std::string toDebugName(const TypeInner &t);
	std::string toDebugName(const FunctionType &t);
};

#endif // COMPILER_IDENTIFIERS