#include "compiler_identifiers.hpp"

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

Id CompilerIdentifiers::getVariableId(const BaseVariable &var)
{
	auto searchResult = vars_.find(var.id);
	if (searchResult != vars_.end())
		return searchResult->second;
	auto id = createId(var.name);
	vars_.insert(std::make_pair(var.id, id));
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
	case EType::Sampler:
	case EType::SampledImage:
		break;
	case EType::Pointer:
	{
		auto ptrType = std::any_cast<PointerType>(t.innerType);
		return toDebugName(ptrType.innerType) + "*"s;
	}
	case EType::Array:
	case EType::Struct:
	case EType::Enum:
	default:
		break;
	}
	return "<unknown_type>";
}

std::string CompilerIdentifiers::toDebugName(const StructureType &t)
{
	return "struct " + t.name;
}

std::string CompilerIdentifiers::toDebugName(const FunctionType &t)
{
	using namespace std::string_literals;
	auto funcTypeDebugName = ""s;
	funcTypeDebugName += t.returnType.debugName + " (";
	for (auto &p : t.paramTypes)
		funcTypeDebugName += p.debugName + ", ";
	funcTypeDebugName.pop_back();
	funcTypeDebugName.pop_back();
	funcTypeDebugName += ")"s;
	return funcTypeDebugName;
}
