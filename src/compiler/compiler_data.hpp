#ifndef COMPILER_DATA
#define COMPILER_DATA

#include <vector>
#include <string>
#include <memory>
#include <any>
#include <cstdint>
#include <spirv.hpp11>

struct Id
{
	uint32_t id;
	std::string debugName;
};

struct FunctionType
{
	Id returnType;
	std::vector<Id> paramTypes;
};

struct StructureType
{
	std::string name;
	std::vector<Id> memberTypes;
};

enum class OpParamType
{
	Id,
	LiteralU,
	LiteralI,
	String,
};

struct OpParam
{
	OpParamType type;
	Id id;
	union
	{
		int64_t inum;
		uint64_t unum;
	};
	uint8_t numSize;
	std::string str;
};

struct SpirVOp
{
	spv::Op op;
	std::vector<OpParam> params;
};

bool operator<(const FunctionType &t1, const FunctionType &t2);
bool operator<(const StructureType &t1, const StructureType &t2);

#endif // COMPILER_DATA