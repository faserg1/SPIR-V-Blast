#ifndef COMPILER_HELPER
#define COMPILER_HELPER

#include <string>
#include <cstdint>
#include <spirv.hpp11>
#include "compiler_data.hpp"

class CompilerHelper
{
public:
	template <class ... TOpParams>
	static SpirVOp op(spv::Op opId, TOpParams ...params)
	{
		SpirVOp op;
		op.op = opId;
		op.params = { params... };
		return op;
	}

	static SpirVOp labelOp(const Id &id);
	static SpirVOp debugOp(const Id &id);
	static SpirVOp debugMemberOp(const Id &id, uint32_t memberPosition, std::string debugMemberName);
	static OpParam paramId(const Id &id);
	static OpParam paramInt(int64_t i, uint8_t size);
	static OpParam paramUint(uint64_t u, uint8_t size);
	static OpParam paramFloat(long double d, uint8_t size);
	static OpParam paramString(std::string str);
};

#endif