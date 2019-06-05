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
	Id getTypeId(const TypeInner &t);
	Id getVariableId(const BaseVariable &var);
private:
	uint32_t counter_;
	std::map<TypeInner, Id> types_;
	std::map<uint64_t, Id> vars_;
private:
	Id createId(const std::string &debugName);
	std::string toDebugName(const TypeInner &t);
};

#endif // COMPILER_IDENTIFIERS