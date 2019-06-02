#include "compiler_identifiers.hpp"

CompilerIdentifiers::CompilerIdentifiers() :
	counter_(0)
{
}

bool CompilerIdentifiers::hasType(const TypeInner &t) const
{
	auto searchResult = types_.find(t);
	return searchResult != types_.end();
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

Id CompilerIdentifiers::createId(const std::string &debugName)
{
	Id id;
	id.id = counter_++;
	id.debugName = debugName;
	return id;
}

std::string CompilerIdentifiers::toDebugName(const TypeInner &t)
{
	switch (t.etype)
	{
	case EType::Int:
	{
		auto intType = std::any_cast<IntType>(t.innerType);
		return "int";
	}
	case EType::Float:
	{
		auto floatType = std::any_cast<FloatType>(t.innerType);
		return "float";
	}
	case EType::Bool:
		return "bool";
	case EType::Void:
		return "void";
	case EType::Matrix:
	case EType::Vector:
	case EType::Image:
	case EType::Sampler:
	case EType::SampledImage:
	case EType::Pointer:
	case EType::Array:
	case EType::Struct:
	case EType::Enum:
	default:
		break;
	}
	return "<unknown_type>";
}