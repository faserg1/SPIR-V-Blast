#include "compiler_common.hpp"
#include "../gen/blast_tokens.hpp"

std::vector<SpirVOp> CompilerCommon::compile(std::shared_ptr<AbstractSyntaxTreeContainer> container, const ShaderPreprocessedInfo &ppInfo)
{
	auto &globalVariables = container->getGlobalVariables();
	for (auto &globalVar : globalVariables)
		compileGlobalVariable(globalVar);

	auto debugOps = ctx_.getDebugNameOps();
	auto decorateOps = ctx_.getDecorateOps();
	auto typeOps = ctx_.getTypeOps();
	auto globalOps = ctx_.getGlobalOps();

	std::vector<SpirVOp> totalOps;
	auto addToTotal = [&totalOps](std::vector<SpirVOp> ops)
	{
		totalOps.insert(totalOps.end(), ops.begin(), ops.end());
	};

	addToTotal(std::move(debugOps));
	addToTotal(std::move(decorateOps));
	addToTotal(std::move(typeOps));
	addToTotal(std::move(globalOps));

	return std::move(totalOps);
}

Id CompilerCommon::compileType(const Type &type)
{
	if (ctx_.hasType(type.innerType))
		return ctx_.getTypeId(type.innerType);
	return compileType(type.innerType);
}

Id CompilerCommon::compileType(const TypeInner &type)
{
	switch (type.etype)
	{
	case EType::Void:
	{
		auto id = ctx_.getTypeId(type);
		SpirVOp op;
		op.op = spv::Op::OpTypeVoid;
		op.params.push_back(paramId(id));
		ctx_.addType(op);
		ctx_.addDebug(debugOp(id));
		return id;
	}
	case EType::Bool:
	{
		auto id = ctx_.getTypeId(type);
		SpirVOp op;
		op.op = spv::Op::OpTypeBool;
		op.params.push_back(paramId(id));
		ctx_.addType(op);
		ctx_.addDebug(debugOp(id));
		return id;
	}
	case EType::Int:
	{
		auto intType = std::any_cast<IntType>(type.innerType);
		auto id = ctx_.getTypeId(type);
		SpirVOp op;
		op.op = spv::Op::OpTypeInt;
		op.params.push_back(paramId(id));
		op.params.push_back(paramUint(intType.width, 32));
		op.params.push_back(paramUint(intType.signedness, 32));
		ctx_.addType(op);
		ctx_.addDebug(debugOp(id));
		return id;
	}
	case EType::Float:
	{
		auto floatType = std::any_cast<FloatType>(type.innerType);
		auto id = ctx_.getTypeId(type);
		SpirVOp op;
		op.op = spv::Op::OpTypeFloat;
		op.params.push_back(paramId(id));
		op.params.push_back(paramUint(floatType.width, 32));
		ctx_.addType(op);
		ctx_.addDebug(debugOp(id));
		return id;
	}
	case EType::Vector:
	{
		auto vectorType = std::any_cast<VectorType>(type.innerType);
		auto componentTypeId = (ctx_.hasType(vectorType.componentType) ? ctx_.getTypeId(vectorType.componentType) : compileType(vectorType.componentType));
		auto id = ctx_.getTypeId(type);
		SpirVOp op;
		op.op = spv::Op::OpTypeVector;
		op.params.push_back(paramId(id));
		op.params.push_back(paramId(componentTypeId));
		op.params.push_back(paramUint(vectorType.componentCount, 32));
		ctx_.addType(op);
		ctx_.addDebug(debugOp(id));
		return id;
	}
	case EType::Matrix:
	{
		auto matrixType = std::any_cast<MatrixType>(type.innerType);
		VectorType vt;
		vt.componentType = matrixType.componentType;
		vt.componentCount = matrixType.rowsCount;
		TypeInner vti;
		vti.etype = EType::Vector;
		vti.innerType = vt;
		auto componentTypeId = (ctx_.hasType(vti) ? ctx_.getTypeId(vti) : compileType(vti));
		auto id = ctx_.getTypeId(type);
		SpirVOp op;
		op.op = spv::Op::OpTypeVector;
		op.params.push_back(paramId(id));
		op.params.push_back(paramId(componentTypeId));
		op.params.push_back(paramUint(matrixType.columnsCount, 32));
		ctx_.addType(op);
		ctx_.addDebug(debugOp(id));
		return id;
	}
	default:
		break;
	}
}

void CompilerCommon::compileGlobalVariable(const GlobalVariable &var)
{
	compileType(var.type);
}

void CompilerCommon::compileFunction(const Function &func)
{
	compileType(func.returnType);
	for (auto &funcParameter : func.parameters)
		compileType(funcParameter.type);
}

SpirVOp CompilerCommon::debugOp(const Id &id)
{
	SpirVOp op;
	op.op = spv::Op::OpName;
	op.params.push_back(paramId(id));
	op.params.push_back(paramString(id.debugName));
	return op;
}

OpParam CompilerCommon::paramId(const Id &id)
{
	OpParam p;
	p.type = OpParamType::Id;
	p.id = id;
	return p;
}

OpParam CompilerCommon::paramInt(int64_t i, uint8_t size)
{
	OpParam p;
	p.type = OpParamType::LiteralI;
	p.inum = i;
	p.numSize = size;
	return p;
}

OpParam CompilerCommon::paramUint(uint64_t u, uint8_t size)
{
	OpParam p;
	p.type = OpParamType::LiteralU;
	p.unum = u;
	p.numSize = size;
	return p;
}

OpParam CompilerCommon::paramString(std::string str)
{
	OpParam p;
	p.type = OpParamType::String;
	p.str = str;
	return p;
}