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

enum class OpParamType
{
	Id,
	Literal,
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
	std::string str;
};

struct SpirVOp
{
	spv::Op op;
	std::vector<OpParam> params;
};

#endif // COMPILER_DATA