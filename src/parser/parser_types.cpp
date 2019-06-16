#include "parser_types.hpp"

bool operator==(const TypeInner &t1, const TypeInner &t2)
{
	if (t1.etype != t2.etype)
		return false;
	switch (t1.etype)
	{
	case EType::Int:
	{
		auto i1 = std::any_cast<IntType>(t1.innerType);
		auto i2 = std::any_cast<IntType>(t2.innerType);
		return i1.width == i2.width && i1.signedness == i2.signedness;
	}
	case EType::Float:
	{
		auto f1 = std::any_cast<FloatType>(t1.innerType);
		auto f2 = std::any_cast<FloatType>(t2.innerType);
		return f1.width == f2.width;
	}
	case EType::Bool:
	case EType::Void:
		return true;
	case EType::Matrix:
	{
		auto m1 = std::any_cast<MatrixType>(t1.innerType);
		auto m2 = std::any_cast<MatrixType>(t2.innerType);
		return m1.rowsCount == m2.rowsCount &&
			m1.columnsCount == m2.columnsCount &&
			m1.componentType == m2.componentType;
	}
	case EType::Vector:
	{
		auto v1 = std::any_cast<VectorType>(t1.innerType);
		auto v2 = std::any_cast<VectorType>(t2.innerType);
		return v1.componentCount == v2.componentCount &&
			v1.componentType == v2.componentType;
	}
	case EType::Image:
	{
		auto i1 = std::any_cast<ImageType>(t1.innerType);
		auto i2 = std::any_cast<ImageType>(t2.innerType);
		return i1.sampledType == i2.sampledType &&
			i1.dimension == i2.dimension &&
			i1.isDepth == i2.isDepth &&
			i1.isArrayed == i2.isArrayed &&
			i1.isMultisampled == i2.isMultisampled &&
			i1.isSampled == i2.isSampled &&
			i1.imageFormat == i2.imageFormat &&
			i1.accessQualifier == i2.accessQualifier;
	}
	case EType::Sampler:
		return true;
	case EType::SampledImage:
	{
		auto si1 = std::any_cast<SampledImageType>(t1.innerType);
		auto si2 = std::any_cast<SampledImageType>(t2.innerType);
		return si1.innerType == si2.innerType;
	}
	case EType::Pointer:
	{
		auto p1 = std::any_cast<PointerType>(t1.innerType);
		auto p2 = std::any_cast<PointerType>(t2.innerType);
		return p1.innerType == p2.innerType && p1.storageClass == p2.storageClass;
	}
	case EType::Array:
	{
		auto a1 = std::any_cast<ArrayType>(t1.innerType);
		auto a2 = std::any_cast<ArrayType>(t2.innerType);
		return a1.isRuntime == a2.isRuntime &&
			a1.length == a2.length &&
			a1.innerType == a2.innerType;
	}
	case EType::Struct:
	{
		auto s1 = std::any_cast<TypeStruct>(t1.innerType);
		auto s2 = std::any_cast<TypeStruct>(t2.innerType);
		return s1.name == s2.name;
	}
	case EType::Enum:
	{
		auto e1 = std::any_cast<TypeEnum>(t1.innerType);
		auto e2 = std::any_cast<TypeEnum>(t2.innerType);
		return e1.name == e2.name;
	}
	}
	return false;
}

bool operator!=(const TypeInner &t1, const TypeInner &t2)
{
	return !(t1 == t2);
}

bool operator<(const TypeInner &t1, const TypeInner &t2)
{
	if (t1.etype != t2.etype)
		return t1.etype < t2.etype;
	switch (t1.etype)
	{
	case EType::Int:
	{
		auto i1 = std::any_cast<IntType>(t1.innerType);
		auto i2 = std::any_cast<IntType>(t2.innerType);
		return i1.width < i2.width || i1.signedness < i2.signedness;
	}
	case EType::Float:
	{
		auto f1 = std::any_cast<FloatType>(t1.innerType);
		auto f2 = std::any_cast<FloatType>(t2.innerType);
		return f1.width < f2.width;
	}
	case EType::Bool:
	case EType::Void:
		return false;
	case EType::Matrix:
	{
		auto m1 = std::any_cast<MatrixType>(t1.innerType);
		auto m2 = std::any_cast<MatrixType>(t2.innerType);
		return m1.rowsCount < m2.rowsCount ||
			m1.columnsCount < m2.columnsCount ||
			m1.componentType < m2.componentType;
	}
	case EType::Vector:
	{
		auto v1 = std::any_cast<VectorType>(t1.innerType);
		auto v2 = std::any_cast<VectorType>(t2.innerType);
		return v1.componentCount < v2.componentCount ||
			v1.componentType < v2.componentType;
	}
	case EType::Image:
	{
		auto i1 = std::any_cast<ImageType>(t1.innerType);
		auto i2 = std::any_cast<ImageType>(t2.innerType);
		return i1.sampledType < i2.sampledType ||
			i1.dimension < i2.dimension ||
			i1.isDepth < i2.isDepth ||
			i1.isArrayed < i2.isArrayed ||
			i1.isMultisampled < i2.isMultisampled ||
			i1.isSampled < i2.isSampled ||
			i1.imageFormat < i2.imageFormat ||
			i1.accessQualifier < i2.accessQualifier;
	}
	case EType::Sampler:
		return false;
	case EType::SampledImage:
	{
		auto si1 = std::any_cast<SampledImageType>(t1.innerType);
		auto si2 = std::any_cast<SampledImageType>(t2.innerType);
		return si1.innerType < si2.innerType;
	}
	case EType::Pointer:
	{
		auto p1 = std::any_cast<PointerType>(t1.innerType);
		auto p2 = std::any_cast<PointerType>(t2.innerType);
		return p1.innerType < p2.innerType || p1.storageClass < p2.storageClass;
	}
	case EType::Array:
	{
		auto a1 = std::any_cast<ArrayType>(t1.innerType);
		auto a2 = std::any_cast<ArrayType>(t2.innerType);
		return a1.isRuntime < a2.isRuntime ||
			a1.length < a2.length ||
			a1.innerType < a2.innerType;
	}
	case EType::Struct:
	{
		auto s1 = std::any_cast<TypeStruct>(t1.innerType);
		auto s2 = std::any_cast<TypeStruct>(t2.innerType);
		return s1.name < s2.name;
	}
	case EType::Enum:
	{
		auto e1 = std::any_cast<TypeEnum>(t1.innerType);
		auto e2 = std::any_cast<TypeEnum>(t2.innerType);
		return e1.name < e2.name;
	}
	}
	return false;
}

bool operator<(const Literal &l1, const Literal &l2)
{
	if (l1.type < l2.type)
		return true;
	if (l1.type == l2.type)
	{
		switch (l1.type)
		{
		case LiteralType::Boolean:
			return l1.boolean < l2.boolean;
		case LiteralType::INumber:
			return l1.inum < l2.inum;
		case LiteralType::UNumber:
			return l1.unum < l2.unum;
		case LiteralType::DNumber:
			return l1.dnum < l2.dnum;
		case LiteralType::String:
			return l1.string < l2.string;
		default:
			break;
		}
	}
	return false;
}

bool operator<(const ConstExpression &e1, const ConstExpression &e2)
{
	if (e1.type.innerType < e2.type.innerType)
		return true;
	if (e1.type.innerType == e2.type.innerType)
	{
		if (e1.literal < e2.literal)
			return true;
		return false;
	}
	return false;
}

bool operator==(const Type &t1, const Type &t2)
{
	return t1.isConst == t2.isConst && t1.innerType == t2.innerType;
}

bool operator!=(const Type &t1, const Type &t2)
{
	return !(t1 == t2);
}