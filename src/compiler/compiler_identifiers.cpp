#include "compiler_identifiers.hpp"
#include "../gen/spirv_enums.hpp"

CompilerIdentifiers::CompilerIdentifiers() :
	counter_(1) // identifiers must start from 1
{
}

bool CompilerIdentifiers::hasType(const TypeInner &t) const
{
	return types_.find(t) != types_.end();
}

bool CompilerIdentifiers::hasType(const StructureType & t) const
{
	return structureTypes_.find(t) != structureTypes_.end();
}

bool CompilerIdentifiers::hasType(const FunctionType &t) const
{
	return functionTypes_.find(t) != functionTypes_.end();
}

bool CompilerIdentifiers::hasConstant(const Type & t, Literal & value) const
{
	ConstExpression e{ t, value };
	return constants_.find(e) != constants_.end();
}

Id CompilerIdentifiers::getTypeId(const TypeInner &t)
{
	auto searchResult = types_.find(t);
	if (searchResult != types_.end())
		return searchResult->second;
	auto id = createId(toDebugName(t));
	types_.insert(std::make_pair(t, id));
	return id;
}

Id CompilerIdentifiers::getTypeId(const StructureType & t)
{
	auto searchResult = structureTypes_.find(t);
	if (searchResult != structureTypes_.end())
		return searchResult->second;
	auto id = createId(toDebugName(t));
	structureTypes_.insert(std::make_pair(t, id));
	return id;
}

Id CompilerIdentifiers::getTypeId(const FunctionType &t)
{
	auto searchResult = functionTypes_.find(t);
	if (searchResult != functionTypes_.end())
		return searchResult->second;
	auto id = createId(toDebugName(t));
	functionTypes_.insert(std::make_pair(t, id));
	return id;
}

Id CompilerIdentifiers::getConstantId(const Type &t, Literal &value)
{
	ConstExpression e{t, value};
	auto searchResult = constants_.find(e);
	if (searchResult != constants_.end())
		return searchResult->second;
	auto id = createId(toDebugName(e));
	constants_.insert(std::make_pair(e, id));
	return id;
}

Id CompilerIdentifiers::getVariableId(const BaseVariable &var)
{
	auto searchResult = vars_.find(var.id);
	if (searchResult != vars_.end())
		return searchResult->second;
	auto id = createId(var.name);
	vars_.insert(std::make_pair(var.id, id));
	return id;
}

Id CompilerIdentifiers::getFunctionId(const Function &func)
{
	auto searchResult = functions_.find(func.id);
	if (searchResult != functions_.end())
		return searchResult->second;
	auto id = createId(func.name);
	functions_.insert(std::make_pair(func.id, id));
	return id;
}

Id CompilerIdentifiers::createId(const std::string &debugName)
{
	Id id;
	id.id = counter_++;
	id.debugName = debugName;
	return id;
}

std::string CompilerIdentifiers::toDebugName(const TypeInner &t)
{
	using namespace std::string_literals;
	switch (t.etype)
	{
	case EType::Int:
	{
		auto intType = std::any_cast<IntType>(t.innerType);
		return "int<"s + std::to_string(intType.width) + ","s + (intType.signedness ? "true"s : "false"s) + ">"s;
	}
	case EType::Float:
	{
		auto floatType = std::any_cast<FloatType>(t.innerType);
		return "float<"s + std::to_string(floatType.width) + ">"s;
	}
	case EType::Bool:
		return "bool";
	case EType::Void:
		return "void";
	case EType::Matrix:
	{
		auto matrixType = std::any_cast<MatrixType>(t.innerType);
		return "mat<"s + toDebugName(matrixType.componentType) + ","s + 
			std::to_string(matrixType.rowsCount) + ","s + std::to_string(matrixType.columnsCount) + ">"s;
	}
	case EType::Vector:
	{
		auto vectorType = std::any_cast<VectorType>(t.innerType);
		return "vec<"s + toDebugName(vectorType.componentType) + ","s + std::to_string(vectorType.componentCount) + ">"s;
	}
	case EType::Image:
	{
		auto b = [](bool b) -> std::string
		{
			return b ? "true"s : "false"s;
		};
		auto imageType = std::any_cast<ImageType>(t.innerType);
		return "img<"s + toDebugName(imageType.sampledType) + ","s + toString(imageType.dimension) + ","s +
			b(imageType.isDepth) + ","s + b(imageType.isArrayed) + ","s + b(imageType.isMultisampled) + ","s +
			std::to_string(imageType.isSampled) + ","s + toString(imageType.imageFormat) +
			(imageType.accessQualifier.has_value() ? (","s + toString(imageType.accessQualifier.value()) + ">"s) : (">"s));
	}
	case EType::Sampler:
	{
		return "smpl"s;
	}
	case EType::SampledImage:
	{
		auto sampledImageType = std::any_cast<SampledImageType>(t.innerType);
		return "simg<"s + toDebugName(sampledImageType.innerType) + ">"s;
	}
	case EType::Pointer:
	{
		auto ptrType = std::any_cast<PointerType>(t.innerType);
		return toDebugName(ptrType.innerType) + "*"s;
	}
	case EType::Array:
	{
		auto arrayType = std::any_cast<ArrayType>(t.innerType);
		return toDebugName(arrayType.innerType) + "["s + (arrayType.isRuntime ? ""s : std::to_string(arrayType.length)) + "]"s;
	}
	case EType::Struct:
	{
		auto structType = std::any_cast<TypeStruct>(t.innerType);
		return "struct "s + structType.name;
	}
	case EType::Enum:
	default:
		break;
	}
	return "<unknown_type>";
}

std::string CompilerIdentifiers::toDebugName(const StructureType &t)
{
	using namespace std::string_literals;
	return "struct "s + t.name;
}

std::string CompilerIdentifiers::toDebugName(const FunctionType &t)
{
	using namespace std::string_literals;
	auto funcTypeDebugName = ""s;
	funcTypeDebugName += t.returnType.debugName + " (";
	if (t.paramTypes.size())
	{
		for (auto &p : t.paramTypes)
			funcTypeDebugName += p.debugName + ", ";
		funcTypeDebugName.pop_back();
		funcTypeDebugName.pop_back();
	}
	funcTypeDebugName += ")"s;
	return funcTypeDebugName;
}

std::string CompilerIdentifiers::toDebugName(const ConstExpression &e)
{
	using namespace std::string_literals;
	//TODO: print literal?
	return "const "s + toDebugName(e.type.innerType);
}
