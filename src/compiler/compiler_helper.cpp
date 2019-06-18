#include "compiler_helper.hpp"

SpirVOp CompilerHelper::labelOp(const Id & id)
{
	SpirVOp op;
	op.op = spv::Op::OpLabel;
	op.params.push_back(paramId(id));
	return op;
}

SpirVOp CompilerHelper::debugOp(const Id &id)
{
	SpirVOp op;
	op.op = spv::Op::OpName;
	op.params.push_back(paramId(id));
	op.params.push_back(paramString(id.debugName));
	return op;
}

SpirVOp CompilerHelper::debugMemberOp(const Id &id, uint32_t memberPosition, std::string debugMemberName)
{
	SpirVOp op;
	op.op = spv::Op::OpMemberName;
	op.params.push_back(paramId(id));
	op.params.push_back(paramUint(memberPosition, 32));
	op.params.push_back(paramString(debugMemberName));
	return op;
}

OpParam CompilerHelper::paramId(const Id &id)
{
	OpParam p;
	p.type = OpParamType::Id;
	p.id = id;
	return p;
}

OpParam CompilerHelper::paramInt(int64_t i, uint8_t size)
{
	OpParam p;
	p.type = OpParamType::LiteralI;
	p.inum = i;
	p.numSize = size;
	return p;
}

OpParam CompilerHelper::paramUint(uint64_t u, uint8_t size)
{
	OpParam p;
	p.type = OpParamType::LiteralU;
	p.unum = u;
	p.numSize = size;
	return p;
}

OpParam CompilerHelper::paramFloat(long double d, uint8_t size)
{
	OpParam p;
	p.type = OpParamType::Float;
	p.dnum = d;
	p.numSize = size;
	return p;
}

OpParam CompilerHelper::paramString(std::string str)
{
	OpParam p;
	p.type = OpParamType::String;
	p.str = str;
	return p;
}