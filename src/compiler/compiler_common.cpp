#include "compiler_common.hpp"
#include "../gen/blast_tokens.hpp"
#include "../shader_preprocessed_info.hpp"

std::vector<SpirVOp> CompilerCommon::compile(std::shared_ptr<AbstractSyntaxTreeContainer> container, const ShaderPreprocessedInfo &ppInfo)
{
	compileMemoryModel(ppInfo.memoryModel(), ppInfo.addressingModel());

	auto &caps = ppInfo.capabilities();
	for (auto cap : caps)
		compileCapability(cap);

	auto &globalVariables = container->getGlobalVariables();
	for (auto &globalVar : globalVariables)
		compileGlobalVariable(globalVar);

	auto &userStructs = container->getStructs();
	for (auto &userStruct : userStructs)
		compileStruct(userStruct);

	auto &functions = container->getFunctions();
	for (auto &function : functions)
		compileFunction(function);

	return std::move(collectResult());
}

void CompilerCommon::applyStorageClass(Type &type, spv::StorageClass storageClass)
{
	switch (type.innerType.etype)
	{
	case EType::Pointer:
	{
		auto &pt = std::any_cast<PointerType>(type.innerType.innerType);
		pt.storageClass = static_cast<uint32_t>(storageClass);
		type.innerType.innerType.swap(std::any(pt));
		break;
	}
	default:
		break;
	}
}

void CompilerCommon::applyStorageClass(BaseVariable &var, spv::StorageClass storageClass)
{
	var.storageClass = static_cast<uint32_t>(storageClass);
	applyStorageClass(var.type, storageClass);
}

Id CompilerCommon::compileType(const Type &type)
{
	if (ctx_.hasType(type.innerType))
		return ctx_.getTypeId(type.innerType);
	return compileType(type.innerType);
}

Id CompilerCommon::compileType(const TypeInner &type)
{
	auto getOrCreateInnerType = [this](const TypeInner &typeInner) -> Id
	{
		return (ctx_.hasType(typeInner) ? ctx_.getTypeId(typeInner) : compileType(typeInner));
	};

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
		auto componentTypeId = getOrCreateInnerType(vectorType.componentType);
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
		auto componentTypeId = getOrCreateInnerType(vti);
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
	case EType::Pointer:
	{
		auto pointerType = std::any_cast<PointerType>(type.innerType);
		auto componentTypeId = getOrCreateInnerType(pointerType.innerType);
		auto id = ctx_.getTypeId(type);
		SpirVOp op;
		op.op = spv::Op::OpTypePointer;
		op.params.push_back(paramId(id));
		op.params.push_back(paramUint(pointerType.storageClass, 32));
		op.params.push_back(paramId(componentTypeId));
		ctx_.addType(op);
		ctx_.addDebug(debugOp(id));
		return id;
	}
	default:
		break;
	}
}

void CompilerCommon::compileMemoryModel(spv::MemoryModel memModel, spv::AddressingModel addrModel)
{
	SpirVOp op;
	op.op = spv::Op::OpMemoryModel;
	op.params.push_back(paramUint(static_cast<uint32_t>(memModel), 32));
	op.params.push_back(paramUint(static_cast<uint32_t>(addrModel), 32));
	ctx_.addHeader(op);
}

void CompilerCommon::compileCapability(spv::Capability cap)
{
	SpirVOp op;
	op.op = spv::Op::OpCapability;
	op.params.push_back(paramUint(static_cast<uint32_t>(cap), 32));
	ctx_.addHeader(op);
}

void CompilerCommon::compileGlobalVariable(GlobalVariable &var)
{
	var.storageClass = UINT32_MAX;

	for (auto &attribute : var.attributes)
	{
		if (attribute.name == "in")
			applyStorageClass(var, spv::StorageClass::Input);
		else if (attribute.name == "out")
			applyStorageClass(var, spv::StorageClass::Output);
	}

	if (var.storageClass == UINT32_MAX)
		applyStorageClass(var, spv::StorageClass::Private);

	auto resultTypeId = compileType(var.type);
	auto id = ctx_.getVariableId(var);
	for (auto &attribute : var.attributes)
	{
		if (attribute.name == "in")
			decorate(id, spv::Decoration::Location, attribute.params);
		else if (attribute.name == "out")
			decorate(id, spv::Decoration::Location, attribute.params);
		else if (attribute.name == "bind")
			decorate(id, spv::Decoration::Binding, attribute.params);
		else if (attribute.name == "builtIn")
		{
			// TODO: [OOKAMI] Read the value from string (as spv::BuiltIn enum)
			//decorate(id, spv::Decoration::BuiltIn, attribute.params);
		}
	}

	SpirVOp op;
	op.op = spv::Op::OpVariable;
	op.params.push_back(paramId(resultTypeId));
	op.params.push_back(paramId(id));
	op.params.push_back(paramUint(var.storageClass, 32));

	// TODO: initializer

	ctx_.addGlobal(op);
	auto opDebug = debugOp(id);
	ctx_.addDebug(opDebug);
}

void CompilerCommon::compileStruct(const Struct &userStruct)
{
	for (auto &member : userStruct.members)
		compileType(member.type);
}

void CompilerCommon::compileFunction(const Function &func)
{
	compileType(func.returnType);
	for (auto &funcParameter : func.parameters)
		compileType(funcParameter.type);
}

void CompilerCommon::decorate(const Id &id, spv::Decoration dec, std::vector<AttributeParam> params)
{
	SpirVOp op;
	op.op = spv::Op::OpDecorate;
	op.params.push_back(paramId(id));
	op.params.push_back(paramUint(static_cast<uint32_t>(dec), 32));
	writeDecorationParams(op, params);
	ctx_.addDecorate(op);
}

void CompilerCommon::decorateMember(const Id &id, uint32_t memberPosition, spv::Decoration dec, std::vector<AttributeParam> params)
{
	SpirVOp op;
	op.op = spv::Op::OpMemberDecorate;
	op.params.push_back(paramId(id));
	op.params.push_back(paramUint(memberPosition, 32));
	op.params.push_back(paramUint(static_cast<uint32_t>(dec), 32));
	writeDecorationParams(op, params);
	ctx_.addDecorate(op);
}

void CompilerCommon::writeDecorationParams(SpirVOp &op, std::vector<AttributeParam> params)
{
	// TODO: write other parameters
	for (auto &param : params)
	{
		switch (param.type)
		{
		case AttributeParamType::Literal:
		{
			switch (param.paramLiteral.type)
			{
			case LiteralType::UNumber:
				op.params.push_back(paramUint(param.paramLiteral.unum, 32));
				break;
			case LiteralType::INumber:
				op.params.push_back(paramInt(param.paramLiteral.inum, 32));
				break;
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
	}
}

std::vector<SpirVOp> CompilerCommon::collectResult()
{
	auto headerOps = ctx_.getHeaderOps();
	auto debugOps = ctx_.getDebugNameOps();
	auto decorateOps = ctx_.getDecorateOps();
	auto typeOps = ctx_.getTypeOps();
	auto constOps = ctx_.getConstOps();
	auto specconstOps = ctx_.getSpecConstOps();
	auto globalOps = ctx_.getGlobalOps();

	std::vector<SpirVOp> totalOps;
	auto addToTotal = [&totalOps](std::vector<SpirVOp> ops)
	{
		totalOps.insert(totalOps.end(), ops.begin(), ops.end());
	};

	addToTotal(std::move(headerOps));
	addToTotal(std::move(debugOps));
	addToTotal(std::move(decorateOps));
	addToTotal(std::move(typeOps));
	addToTotal(std::move(constOps));
	addToTotal(std::move(specconstOps));
	addToTotal(std::move(globalOps));

	return std::move(totalOps);
}

SpirVOp CompilerCommon::debugOp(const Id &id)
{
	SpirVOp op;
	op.op = spv::Op::OpName;
	op.params.push_back(paramId(id));
	op.params.push_back(paramString(id.debugName));
	return op;
}

SpirVOp CompilerCommon::debugMemberOp(const Id &id, uint32_t memberPosition)
{
	SpirVOp op;
	op.op = spv::Op::OpMemberName;
	op.params.push_back(paramId(id));
	op.params.push_back(paramUint(memberPosition, 32));
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