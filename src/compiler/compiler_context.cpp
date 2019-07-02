#include "compiler_context.hpp"

bool CompilerContext::hasType(const TypeInner &type) const
{
	return ids_.hasType(type);
}

bool CompilerContext::hasType(const StructureType & type) const
{
	return ids_.hasType(type);
}

bool CompilerContext::hasType(const FunctionType &funcType) const
{
	return ids_.hasType(funcType);
}

bool CompilerContext::hasConstant(const Type &t, Literal &value) const
{
	return ids_.hasConstant(t, value);
}

Id CompilerContext::getTypeId(const TypeInner &type)
{
	return ids_.getTypeId(type);
}

Id CompilerContext::getTypeId(const StructureType &type)
{
	return ids_.getTypeId(type);
}

Id CompilerContext::getTypeId(const FunctionType &funcType)
{
	return ids_.getTypeId(funcType);
}

Id CompilerContext::getConstantId(const Type &t, Literal &value)
{
	return ids_.getConstantId(t, value);
}

Id CompilerContext::getVariableId(const BaseVariable & var)
{
	return ids_.getVariableId(var);
}

Id CompilerContext::getFunctionId(const Function &func)
{
	return ids_.getFunctionId(func);
}

Id CompilerContext::findIdentifier(const Identifier &ident)
{
	return ids_.findIdentifier(ident);
}

void CompilerContext::addConstantIdAssociation(const BaseVariable &var, const Id &id)
{
	ids_.addConstantIdAssociation(var, id);
}

Id CompilerContext::createId(const std::string debugName)
{
	return ids_.createId(debugName);
}

void CompilerContext::addHeader(SpirVOp headerInstruction)
{
	headerOps_.push_back(headerInstruction);
}

void CompilerContext::addDecorate(SpirVOp decorateInstruction)
{
	decorateOps_.push_back(decorateInstruction);
}

void CompilerContext::addType(SpirVOp typeInstruction)
{
	typeOps_.push_back(typeInstruction);
}

void CompilerContext::addDebug(SpirVOp debugInstruction)
{
	debugNameOps_.push_back(debugInstruction);
}

void CompilerContext::addConst(SpirVOp constInstruction)
{
	constOps_.push_back(constInstruction);
}

void CompilerContext::addSpecConst(SpirVOp specConstInstruction)
{
	specConstOps_.push_back(specConstInstruction);
}

void CompilerContext::addGlobal(SpirVOp globalInstruction)
{
	globalOps_.push_back(globalInstruction);
}

std::vector<SpirVOp> CompilerContext::getHeaderOps() const
{
	return headerOps_;
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

std::vector<SpirVOp> CompilerContext::getConstOps() const
{
	return constOps_;
}

std::vector<SpirVOp> CompilerContext::getSpecConstOps() const
{
	return specConstOps_;
}

std::vector<SpirVOp> CompilerContext::getGlobalOps() const
{
	return globalOps_;
}