#include "compiler_context.hpp"

bool CompilerContext::hasType(const TypeInner &type)
{
	return ids_.hasType(type);
}

Id CompilerContext::getTypeId(const TypeInner &type)
{
	return ids_.getTypeId(type);
}

void CompilerContext::addType(SpirVOp typeInstruction)
{
	typeOps_.push_back(typeInstruction);
}

void CompilerContext::addDebug(SpirVOp debugInstruction)
{
	debugNameOps_.push_back(debugInstruction);
}