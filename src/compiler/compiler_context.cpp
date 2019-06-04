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

std::vector<SpirVOp> CompilerContext::getDebugNameOps() const
{
	return debugNameOps_;
}

std::vector<SpirVOp> CompilerContext::getDecorateOps() const
{
	return decorateOps_;
}

std::vector<SpirVOp> CompilerContext::getTypeOps() const
{
	return typeOps_;
}

std::vector<SpirVOp> CompilerContext::getGlobalOps() const
{
	return globalOps_;
}