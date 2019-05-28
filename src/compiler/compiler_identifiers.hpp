#ifndef COMPILER_IDENTIFIERS
#define COMPILER_IDENTIFIERS

#include <string>
#include <cstdint>
#include <map>
#include "compiler_data.hpp"

class CompilerIdentifiers
{
public:
	CompilerIdentifiers();
	
	Id getTypeId(const Type &t);
private:
	uint32_t counter_;
	std::map<TypeInner, Id> types_;
private:
	Id createId(const std::string &debugName);
	Id getTypeId(const TypeInner &t);
	std::string toDebugName(const TypeInner &t);
};

#endif // COMPILER_IDENTIFIERS