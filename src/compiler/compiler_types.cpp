#include "compiler_types.hpp"
#include <stdexcept>

ECompilerType CompilerType::type() const
{
	return type_;
}

bool CompilerType::operator==(const CompilerType &otherType) const
{
	if (type() != otherType.type())
		return false;
	return isEqual(otherType);
}

CompilerType::CompilerType(ECompilerType type) :
	type_(type)
{
}

BasicType::BasicType(ECompilerType type)
	: CompilerType(type)
{
}

IntegerType::IntegerType(int16_t width, int16_t signedness) :
	BasicType(ECompilerType::Integer), width_(width), signedness_(signedness)
{
}

VoidType::VoidType() :
	BasicType(ECompilerType::Void)
{

}

bool VoidType::isEqual(const CompilerType &otherType) const
{
	if (otherType.type() != ECompilerType::Void)
		throw std::runtime_error("VoidType::isEqual: other type cannot be non void!");
	return true;
}

BooleanType::BooleanType() :
	BasicType(ECompilerType::Boolean)
{

}

bool BooleanType::isEqual(const CompilerType &otherType) const
{
	if (otherType.type() != ECompilerType::Boolean)
		throw std::runtime_error("VoidType::isEqual: other type cannot be non void!");
	return true;
}

int16_t IntegerType::width() const
{
	return width_;
}

int16_t IntegerType::signedness() const
{
	return signedness_;
}

bool IntegerType::isEqual(const CompilerType &otherType) const
{
	auto pOtherType = dynamic_cast<const IntegerType*>(&otherType);
	return width() == pOtherType->width() && signedness() == pOtherType->signedness();
}

FloatType::FloatType(int16_t width) :
	BasicType(ECompilerType::Float), width_(width)
{
}

int16_t FloatType::width() const
{
	return width_;
}

bool FloatType::isEqual(const CompilerType &otherType) const
{
	auto pOtherType = dynamic_cast<const FloatType*>(&otherType);
	return width() == pOtherType->width();
}
