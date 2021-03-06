#include "compiler_common.hpp"
#include <set>
#include "../gen/blast_tokens.hpp"
#include "../gen/spirv_enums.hpp"
#include "../shader_preprocessed_info.hpp"
#include "compiler_helper.hpp"
#include "compiler_body_compilation_context.hpp"

std::vector<SpirVOp> CompilerCommon::compile(std::shared_ptr<AbstractSyntaxTreeContainer> container, const ShaderPreprocessedInfo &ppInfo)
{
	compileMemoryModel(ppInfo.memoryModel(), ppInfo.addressingModel());

	auto caps = ppInfo.capabilities();
	for (auto cap : caps)
		compileCapability(cap);

	auto userStructs = container->getStructs();
	for (auto &userStruct : userStructs)
		compileStruct(userStruct);

	auto globalVariables = container->getGlobalVariables();
	for (auto &globalVar : globalVariables)
		compileGlobalVariable(globalVar);

	auto functions = container->getFunctions();
	for (auto &function : functions)
		compileFunction(function);

	return std::move(collectResult());
}

void CompilerCommon::applyStorageClass(TypeInner &type, spv::StorageClass storageClass)
{
	switch (type.etype)
	{
	case EType::Pointer:
	{
		auto pt = std::any_cast<PointerType>(type.innerType);
		pt.storageClass = static_cast<uint32_t>(storageClass);
		applyStorageClass(pt.innerType, storageClass);
		type.innerType.emplace<PointerType>(pt);
		break;
	}
	default:
		break;
	}
}

void CompilerCommon::applyStorageClass(BaseVariable &var, spv::StorageClass storageClass)
{
	var.storageClass = static_cast<uint32_t>(storageClass);
	applyStorageClass(var.type.innerType, storageClass);
}

void CompilerCommon::compileStorageClass(GlobalVariable &var)
{
	var.storageClass = UINT32_MAX;

	for (auto &attribute : var.attributes)
	{
		if (attribute.name == "in" && !var.type.isConst)
			applyStorageClass(var, spv::StorageClass::Input);
		else if (attribute.name == "out" && !var.type.isConst)
			applyStorageClass(var, spv::StorageClass::Output);
		else if (attribute.name == "push" && !var.type.isConst)
			applyStorageClass(var, spv::StorageClass::PushConstant);
	}

	if (var.storageClass == UINT32_MAX)
		applyStorageClass(var, spv::StorageClass::Private);
}

void CompilerCommon::compileDecorations(const Id &id, GlobalVariable &var)
{
	for (auto &attribute : var.attributes)
	{
		// Non-constant decorations
		if (attribute.name == "in" && !var.type.isConst)
			decorate(id, spv::Decoration::Location, attribute.params);
		else if (attribute.name == "out" && !var.type.isConst)
			decorate(id, spv::Decoration::Location, attribute.params);
		else if (attribute.name == "bind" && !var.type.isConst)
			decorate(id, spv::Decoration::Binding, attribute.params);
		else if (attribute.name == "builtIn" && !var.type.isConst)
		{
			auto &bType = attribute.params[0];
			if (bType.type == AttributeParamType::Identifier)
			{
				auto builtId = fromString<spv::BuiltIn>(bType.paramIdent);
				bType.paramLiteral.unum = static_cast<uint32_t>(builtId);
				bType.paramLiteral.type = LiteralType::UNumber;
				bType.type = AttributeParamType::Literal;
			}
			decorate(id, spv::Decoration::BuiltIn, attribute.params);
		}
		//Constant decoration
		else if (attribute.name == "spec" && var.type.isConst)
			decorate(id, spv::Decoration::SpecId, attribute.params);
	}
}

void CompilerCommon::compileDecorations(const Id &structId, const Struct &userStruct)
{
	for (auto &attribute : userStruct.attributes)
	{
		if (attribute.name == "block")
			decorate(structId, spv::Decoration::Block, attribute.params);
		if (attribute.name == "bufferBlock")
			decorate(structId, spv::Decoration::BufferBlock, attribute.params);
	}
}

void CompilerCommon::compileDecorations(const Id &structId, uint32_t memberPosition, const StructMember &member)
{
	for (auto &attribute : member.attributes)
	{
		if (attribute.name == "rowMajor")
			decorateMember(structId, memberPosition, spv::Decoration::RowMajor, attribute.params);
		else if (attribute.name == "colMajor")
			decorateMember(structId, memberPosition, spv::Decoration::ColMajor, attribute.params);
		else if (attribute.name == "matrixStride")
			decorateMember(structId, memberPosition, spv::Decoration::MatrixStride, attribute.params);
		else if (attribute.name == "component")
			decorateMember(structId, memberPosition, spv::Decoration::Component, attribute.params);
		else if (attribute.name == "offset")
			decorateMember(structId, memberPosition, spv::Decoration::Offset, attribute.params);
	}
}

Id CompilerCommon::compileType(const Type &type)
{
	if (ctx_.hasType(type.innerType))
		return ctx_.getTypeId(type.innerType);
	return compileType(type.innerType);
}

Id CompilerCommon::compileType(const FunctionType &funcType)
{
	if (ctx_.hasType(funcType))
		return ctx_.getTypeId(funcType);
	auto funcTypeId = ctx_.getTypeId(funcType);
	SpirVOp opType;
	opType.op = spv::Op::OpTypeFunction;
	opType.params.push_back(CompilerHelper::paramId(funcTypeId));
	opType.params.push_back(CompilerHelper::paramId(funcType.returnType));
	for (auto &idParam : funcType.paramTypes)
		opType.params.push_back(CompilerHelper::paramId(idParam));
	ctx_.addType(opType);
	ctx_.addDebug(CompilerHelper::debugOp(funcTypeId));
	return funcTypeId;
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
		op.params.push_back(CompilerHelper::paramId(id));
		ctx_.addType(op);
		ctx_.addDebug(CompilerHelper::debugOp(id));
		return id;
	}
	case EType::Bool:
	{
		auto id = ctx_.getTypeId(type);
		SpirVOp op;
		op.op = spv::Op::OpTypeBool;
		op.params.push_back(CompilerHelper::paramId(id));
		ctx_.addType(op);
		ctx_.addDebug(CompilerHelper::debugOp(id));
		return id;
	}
	case EType::Int:
	{
		auto intType = std::any_cast<IntType>(type.innerType);
		auto id = ctx_.getTypeId(type);
		SpirVOp op;
		op.op = spv::Op::OpTypeInt;
		op.params.push_back(CompilerHelper::paramId(id));
		op.params.push_back(CompilerHelper::paramUint(intType.width, 32));
		op.params.push_back(CompilerHelper::paramUint(intType.signedness, 32));
		ctx_.addType(op);
		ctx_.addDebug(CompilerHelper::debugOp(id));
		return id;
	}
	case EType::Float:
	{
		auto floatType = std::any_cast<FloatType>(type.innerType);
		auto id = ctx_.getTypeId(type);
		SpirVOp op;
		op.op = spv::Op::OpTypeFloat;
		op.params.push_back(CompilerHelper::paramId(id));
		op.params.push_back(CompilerHelper::paramUint(floatType.width, 32));
		ctx_.addType(op);
		ctx_.addDebug(CompilerHelper::debugOp(id));
		return id;
	}
	case EType::Vector:
	{
		auto vectorType = std::any_cast<VectorType>(type.innerType);
		auto componentTypeId = getOrCreateInnerType(vectorType.componentType);
		auto id = ctx_.getTypeId(type);
		SpirVOp op;
		op.op = spv::Op::OpTypeVector;
		op.params.push_back(CompilerHelper::paramId(id));
		op.params.push_back(CompilerHelper::paramId(componentTypeId));
		op.params.push_back(CompilerHelper::paramUint(vectorType.componentCount, 32));
		ctx_.addType(op);
		ctx_.addDebug(CompilerHelper::debugOp(id));
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
		op.params.push_back(CompilerHelper::paramId(id));
		op.params.push_back(CompilerHelper::paramId(componentTypeId));
		op.params.push_back(CompilerHelper::paramUint(matrixType.columnsCount, 32));
		ctx_.addType(op);
		ctx_.addDebug(CompilerHelper::debugOp(id));
		return id;
	}
	case EType::Array:
	{
		auto arrayType = std::any_cast<ArrayType>(type.innerType);
		auto componentTypeId = getOrCreateInnerType(arrayType.innerType);
		auto id = ctx_.getTypeId(type);
		SpirVOp op;
		op.op = (arrayType.isRuntime ? spv::Op::OpTypeRuntimeArray : spv::Op::OpTypePointer);
		op.params.push_back(CompilerHelper::paramId(id));
		op.params.push_back(CompilerHelper::paramId(componentTypeId));
		if (!arrayType.isRuntime)
		{
			if (arrayType.lengthId.id)
			{
				//op.params.push_back(CompilerHelper::paramId(arrayType.lengthId));
			}
			//op.params.push_back(CompilerHelper::paramId());
		}
		
		ctx_.addType(op);
	}
	case EType::Pointer:
	{
		auto pointerType = std::any_cast<PointerType>(type.innerType);
		auto componentTypeId = getOrCreateInnerType(pointerType.innerType);
		auto id = ctx_.getTypeId(type);
		SpirVOp op;
		op.op = spv::Op::OpTypePointer;
		op.params.push_back(CompilerHelper::paramId(id));
		op.params.push_back(CompilerHelper::paramUint(pointerType.storageClass, 32));
		op.params.push_back(CompilerHelper::paramId(componentTypeId));
		ctx_.addType(op);
		ctx_.addDebug(CompilerHelper::debugOp(id));
		return id;
	}
	case EType::Struct:
	{
		auto structType = std::any_cast<TypeStruct>(type.innerType);
		auto it = structIds_.find(structType.name);
		auto id = it->second;
		ctx_.addDebug(CompilerHelper::debugOp(id));
		return id;
	}
	default:
		break;
	}
	throw std::runtime_error("Unsupported type");
}

void CompilerCommon::compileMemoryModel(spv::MemoryModel memModel, spv::AddressingModel addrModel)
{
	SpirVOp op;
	op.op = spv::Op::OpMemoryModel;
	op.params.push_back(CompilerHelper::paramUint(static_cast<uint32_t>(memModel), 32));
	op.params.push_back(CompilerHelper::paramUint(static_cast<uint32_t>(addrModel), 32));
	ctx_.addHeader(op);
}

void CompilerCommon::compileCapability(spv::Capability cap)
{
	SpirVOp op;
	op.op = spv::Op::OpCapability;
	op.params.push_back(CompilerHelper::paramUint(static_cast<uint32_t>(cap), 32));
	ctx_.addHeader(op);
}

void CompilerCommon::compileGlobalVariable(GlobalVariable &var)
{
	compileStorageClass(var);

	auto resultTypeId = compileType(var.type);
	Id id;
	SpirVOp op;

	bool isSpecConst = var.type.isConst && std::find_if(var.attributes.begin(), var.attributes.end(), [](const Attribute &attr) -> bool
	{
		return attr.name == "spec";
	}) != var.attributes.end();

	if (var.type.isConst)
	{
		if (!var.initialization.has_value())
		{
			id = ctx_.getConstantId(var.type, bl);
			op.op = spv::Op::OpConstantNull;
			op.params.push_back(CompilerHelper::paramId(resultTypeId));
			op.params.push_back(CompilerHelper::paramId(id));

			// TODO: Check if has null constant already
		}
		else
		{
			const auto etype = var.type.innerType.etype;
			const std::set<EType> scalarTypes = { EType::Int, EType::Float };
			const std::set<EType> compositeTypes = { EType::Vector, EType::Matrix, EType::Array, EType::Struct };
			if (etype == EType::Bool)
			{
				Literal bl;
				auto expression = std::get<ScalarInitialization>(var.initialization.value().value);
				compileConstExpression(expression, bl);
				if (!isSpecConst && ctx_.hasConstant(var.type, bl))
					return;
				op.op = (bl.boolean ?
					(isSpecConst ? spv::Op::OpSpecConstantTrue : spv::Op::OpConstantTrue) :
					(isSpecConst ? spv::Op::OpSpecConstantFalse : spv::Op::OpConstantFalse));
				id = isSpecConst ? ctx_.getVariableId(var) : ctx_.getConstantId(var.type, bl);
				op.params.push_back(CompilerHelper::paramId(resultTypeId));
				op.params.push_back(CompilerHelper::paramId(id));
			}
			else if (scalarTypes.find(etype) != scalarTypes.end())
			{
				Literal bl;
				auto expression = std::get<ScalarInitialization>(var.initialization.value().value);
				compileConstExpression(expression, bl);
				bl = castToType(var.type, bl);
				if (!isSpecConst && ctx_.hasConstant(var.type, bl))
					return;
				op.op = (isSpecConst ? spv::Op::OpSpecConstant : spv::Op::OpConstant);
				id = isSpecConst ? ctx_.getVariableId(var) : ctx_.getConstantId(var.type, bl);
				op.params.push_back(CompilerHelper::paramId(resultTypeId));
				op.params.push_back(CompilerHelper::paramId(id));
				switch (etype)
				{
				case EType::Int:
				{
					auto intType = std::any_cast<IntType>(var.type.innerType.innerType);
					op.params.push_back(intType.signedness ? CompilerHelper::paramInt(bl.inum, intType.width) : CompilerHelper::paramUint(bl.unum, intType.width));
					break;
				}
				case EType::Float:
				{
					auto floatType = std::any_cast<FloatType>(var.type.innerType.innerType);
					op.params.push_back(CompilerHelper::paramFloat(bl.dnum, floatType.width));
					break;
				}
				default:
					throw std::runtime_error("Unsupported scalar type for constant creation");
				}
			}
			else if (compositeTypes.find(etype) != compositeTypes.end())
			{
				throw std::runtime_error("Unsupported type for constant creation");
			}
			else
			{
				throw std::runtime_error("Unsupported type for constant creation");
			}
			ctx_.addConstantIdAssociation(var, id);
		}
	}
	else
	{
		id = ctx_.getVariableId(var);
		op.op = spv::Op::OpVariable;
		op.params.push_back(CompilerHelper::paramId(resultTypeId));
		op.params.push_back(CompilerHelper::paramId(id));
		op.params.push_back(CompilerHelper::paramUint(var.storageClass, 32));

		// TODO: initializer
	}

	compileDecorations(id, var);

	if (var.type.isConst)
	{
		if (isSpecConst)
			ctx_.addSpecConst(op);
		else
			ctx_.addConst(op);
	}
	else
		ctx_.addGlobal(op);
	auto opDebug = CompilerHelper::debugOp(id);
	ctx_.addDebug(opDebug);
}

void CompilerCommon::compileStruct(const Struct &userStruct)
{
	StructureType sType;
	sType.name = userStruct.name;
	for (auto &member : userStruct.members)
		sType.memberTypes.push_back(compileType(member.type));
	auto id = ctx_.getTypeId(sType);

	compileDecorations(id, userStruct);

	SpirVOp op;
	op.op = spv::Op::OpTypeStruct;
	op.params.push_back(CompilerHelper::paramId(id));
	for (uint32_t i = 0; i < userStruct.members.size(); i++)
	{
		op.params.push_back(CompilerHelper::paramId(sType.memberTypes[i]));
		compileDecorations(id, i, userStruct.members[i]);
		auto dOp = CompilerHelper::debugMemberOp(id, i, userStruct.members[i].name);
		ctx_.addDebug(dOp);
	}
	ctx_.addType(op);
	ctx_.addDebug(CompilerHelper::debugOp(id));
	structIds_.insert(std::make_pair(userStruct.name, id));
	structs_.insert(std::make_pair(id, userStruct));
}

void CompilerCommon::compileFunction(const Function &func)
{
	// Compile function type
	FunctionType funcType;
	funcType.returnType = compileType(func.returnType);
	for (auto &funcParameter : func.parameters)
		funcType.paramTypes.push_back(compileType(funcParameter.type));
	auto funcTypeId = compileType(funcType);
	// Compile function
	SpirVOp opFunc, opFuncEnd;
	std::vector<SpirVOp> opParameters;
	opFunc.op = spv::Op::OpFunction;
	auto id = ctx_.getFunctionId(func);
	opFunc.params.push_back(CompilerHelper::paramId(funcType.returnType));
	opFunc.params.push_back(CompilerHelper::paramId(id));
	opFunc.params.push_back(CompilerHelper::paramUint(0, 32)); //TODO: [OOKAMI] Function control
	opFunc.params.push_back(CompilerHelper::paramId(funcTypeId));

	auto bodyOps = compileFunctionBody(func, id);

	opFuncEnd.op = spv::Op::OpFunctionEnd;

	for (size_t paramIdx = 0; paramIdx < func.parameters.size(); paramIdx++)
	{
		SpirVOp opFuncParam;
		opFuncParam.op = spv::Op::OpFunctionParameter;
		auto resultTypeId = funcType.paramTypes[paramIdx];
		auto id = ctx_.getVariableId(func.parameters[paramIdx]);
		opFuncParam.params.push_back(CompilerHelper::paramId(resultTypeId));
		opFuncParam.params.push_back(CompilerHelper::paramId(id));
		opParameters.push_back(opFuncParam);
		ctx_.addDebug(CompilerHelper::debugOp(id));
	}

	ctx_.addGlobal(opFunc);
	for (auto &opParam : opParameters)
		ctx_.addGlobal(opParam);
	for (auto &opBody : bodyOps)
		ctx_.addGlobal(opBody);
	ctx_.addGlobal(opFuncEnd);
	ctx_.addDebug(CompilerHelper::debugOp(id));
}

std::vector<SpirVOp> CompilerCommon::compileFunctionBody(const Function &func, const Id &funcId)
{
	using namespace std::string_literals;
	auto bodyCtx = CompilerBodyCompilationContext::create(&ctx_);
	auto idLabelStart = bodyCtx->createLabelId(funcId.debugName + "_start"s);
	auto opLabelStart = CompilerHelper::labelOp(idLabelStart);
	std::vector<SpirVOp> ops;
	ops.push_back(opLabelStart);

	if (func.body)
	{
		auto bodyOps = compileExpression(func.body.value(), bodyCtx);
		ops.insert(ops.end(), bodyOps.begin(), bodyOps.end());
	}

	return std::move(ops);
}

std::vector<SpirVOp> CompilerCommon::compileExpression(const Expression &expression, std::shared_ptr<CompilerBodyCompilationContext> parentContext)
{
	using namespace std::string_literals;
	std::vector<SpirVOp> ops;
	const auto append = [&ops](std::vector<SpirVOp> opsToInsert)
	{
		ops.insert(ops.end(), opsToInsert.begin(), opsToInsert.end());
	};
	auto bodyCtx = CompilerBodyCompilationContext::create(&ctx_, parentContext);
	switch (expression.type)
	{
	case ExpressionType::StatementGroup:
		for (auto statement : expression.params)
		{
			auto statementOps = compileExpression(statement, bodyCtx);
			ops.insert(ops.end(), statementOps.begin(), statementOps.end());
		}
		break;
	case ExpressionType::If:
	{
		auto it = expression.params.begin();
		Expression init;
		if (expression.params.size() == 3)
		{
			init = *it;
			it++;
		}
		Expression cond = *it++;
		Expression doIfTrue = *it++;
		break;
	}
	case ExpressionType::IfElse:
	{
		auto it = expression.params.begin();
		Expression init;
		if (expression.params.size() == 4)
		{
			init = *it;
			it++;
			append(compileExpression(init, bodyCtx));
		}
		Expression cond = *it++;
		Expression doIfTrue = *it++;
		Expression doIfFalse = *it++;

		const auto idTrue = bodyCtx->createLabelId("cond_true"s);
		const auto labelTrue = CompilerHelper::labelOp(idTrue);
		const auto idFalse = bodyCtx->createLabelId("cond_false"s);
		const auto labelFalse = CompilerHelper::labelOp(idFalse);
		const auto idMerge = bodyCtx->createLabelId("cond_stop"s);
		const auto labelMerge = CompilerHelper::labelOp(idMerge);


		append(compileExpression(cond, bodyCtx));
		const auto selectionMerge = CompilerHelper::op(spv::Op::OpSelectionMerge,
			CompilerHelper::paramId(idMerge), CompilerHelper::paramInt((int64_t) spv::SelectionControlMask::MaskNone, 32));
		

		ops.push_back(labelTrue);
		append(compileExpression(doIfTrue, bodyCtx));
		ops.push_back(labelFalse);
		append(compileExpression(doIfFalse, bodyCtx));
		ops.push_back(labelMerge);
		break;
	}
	}
	return std::move(ops);
}

void CompilerCommon::compileConstExpression(const Expression &expression, Literal &l)
{
	switch (expression.type)
	{
	case ExpressionType::Literal:
	{
		l = expression.literal;
		if (l.type == LiteralType::String)
			l.boolean = !l.string.empty();
		return;
	}
	case ExpressionType::And:
	{
		Literal l1, l2;
		l.type = LiteralType::Boolean;
		compileConstExpression(expression.params[0], l1);
		if (!l1.boolean)
		{
			l.boolean = false;
			return;
		}
		compileConstExpression(expression.params[1], l2);
		l.boolean = l2.boolean;
		return;
	}
	case ExpressionType::Or:
	{
		Literal l1, l2;
		l.type = LiteralType::Boolean;
		compileConstExpression(expression.params[0], l1);
		if (l1.boolean)
		{
			l.boolean = true;
			return;
		}
		compileConstExpression(expression.params[1], l2);
		l.boolean = l2.boolean;
		return;
	}
	case ExpressionType::Not:
	{
		Literal l1;
		l.type = LiteralType::Boolean;
		compileConstExpression(expression.params[0], l1);
		l.boolean = !l1.boolean;
		return;
	}
	case ExpressionType::Less:
	{
		Literal l1, l2;
		l.type = LiteralType::Boolean;
		compileConstExpression(expression.params[0], l1);
		compileConstExpression(expression.params[1], l2);
		l.boolean = (l1 < l2);
		return;
	}
	case ExpressionType::More:
	{
		Literal l1, l2;
		l.type = LiteralType::Boolean;
		compileConstExpression(expression.params[0], l1);
		compileConstExpression(expression.params[1], l2);
		l.boolean = (l1 > l2);
		return;
	}
	case ExpressionType::LessOrEqual:
	{
		Literal l1, l2;
		l.type = LiteralType::Boolean;
		compileConstExpression(expression.params[0], l1);
		compileConstExpression(expression.params[1], l2);
		l.boolean = (l1 <= l2);
		return;
	}
	case ExpressionType::MoreOrEqual:
	{
		Literal l1, l2;
		l.type = LiteralType::Boolean;
		compileConstExpression(expression.params[0], l1);
		compileConstExpression(expression.params[1], l2);
		l.boolean = (l1 >= l2);
		return;
	}
	case ExpressionType::Equal:
	{
		Literal l1, l2;
		l.type = LiteralType::Boolean;
		compileConstExpression(expression.params[0], l1);
		compileConstExpression(expression.params[1], l2);
		l.boolean = (l1 == l2);
		return;
	}
	case ExpressionType::NonEqual:
	{
		Literal l1, l2;
		l.type = LiteralType::Boolean;
		compileConstExpression(expression.params[0], l1);
		compileConstExpression(expression.params[1], l2);
		l.boolean = (l1 != l2);
		return;
	}
	default:
		break;
	}
}

void CompilerCommon::compileConstExpression(const Expression &expression, Identifier &i)
{

}

Literal CompilerCommon::castToType(const Type &t, const Literal &l)
{
	Literal r;
	switch (t.innerType.etype)
	{
	case EType::Bool:
		r.type = LiteralType::Boolean;
		switch (l.type)
		{
		case LiteralType::DNumber:
			r.boolean = l.dnum != 0;
			break;
		default:
			r.boolean = l.boolean;
			break;
		}
		break;
	case EType::Int:
	{
		auto intType = std::any_cast<IntType>(t.innerType.innerType);
		r.type = (intType.signedness ? LiteralType::INumber : LiteralType::UNumber);
		switch (l.type)
		{
		case LiteralType::DNumber:
			if (intType.signedness)
				r.inum = static_cast<int64_t>(l.dnum);
			else
				r.unum = static_cast<uint64_t>(l.dnum);
			break;
		case LiteralType::String:
			throw std::runtime_error("Invalid literal cast");
		default:
			r.unum = l.unum;
			break;
		}
		break;
	}
	case EType::Float:
		r.type = LiteralType::DNumber;
		switch (l.type)
		{
		case LiteralType::INumber:
			r.dnum = static_cast<long double>(l.inum);
			break;
		case LiteralType::UNumber:
			r.dnum = static_cast<long double>(l.unum);
			break;
		case LiteralType::String:
			throw std::runtime_error("Invalid literal cast");
		default:
			r.dnum = l.dnum;
			break;
		}
		break;
	default:
		throw std::runtime_error("Invalid literal cast");
	}
	return r;
}

void CompilerCommon::decorate(const Id &id, spv::Decoration dec, std::vector<AttributeParam> params)
{
	SpirVOp op;
	op.op = spv::Op::OpDecorate;
	op.params.push_back(CompilerHelper::paramId(id));
	op.params.push_back(CompilerHelper::paramUint(static_cast<uint32_t>(dec), 32));
	writeDecorationParams(op, params);
	ctx_.addDecorate(op);
}

void CompilerCommon::decorateMember(const Id &id, uint32_t memberPosition, spv::Decoration dec, std::vector<AttributeParam> params)
{
	SpirVOp op;
	op.op = spv::Op::OpMemberDecorate;
	op.params.push_back(CompilerHelper::paramId(id));
	op.params.push_back(CompilerHelper::paramUint(memberPosition, 32));
	op.params.push_back(CompilerHelper::paramUint(static_cast<uint32_t>(dec), 32));
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
				op.params.push_back(CompilerHelper::paramUint(param.paramLiteral.unum, 32));
				break;
			case LiteralType::INumber:
				op.params.push_back(CompilerHelper::paramInt(param.paramLiteral.inum, 32));
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