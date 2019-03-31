#include "parser_types.hpp"
#include <stdexcept>

EBlastType BlastType::type() const
{
	return type_;
}

bool BlastType::operator==(const BlastType &otherType) const
{
	if (type() != otherType.type())
		return false;
	return isEqual(otherType);
}

BlastType::BlastType(EBlastType type) :
	type_(type)
{
}

BasicBlastType::BasicBlastType(EBlastType type)
	: BlastType(type)
{
}

IntegerBlastType::IntegerBlastType(int16_t width, int16_t signedness) :
	BasicBlastType(EBlastType::Integer), width_(width), signedness_(signedness)
{
}

VoidBlastType::VoidBlastType() :
	BasicBlastType(EBlastType::Void)
{

}

bool VoidBlastType::isEqual(const BlastType &otherType) const
{
	if (otherType.type() != EBlastType::Void)
		throw std::runtime_error("VoidType::isEqual: other type cannot be non void!");
	return true;
}

BooleanBlastType::BooleanBlastType() :
	BasicBlastType(EBlastType::Boolean)
{

}

bool BooleanBlastType::isEqual(const BlastType &otherType) const
{
	if (otherType.type() != EBlastType::Boolean)
		throw std::runtime_error("VoidType::isEqual: other type cannot be non void!");
	return true;
}

int16_t IntegerBlastType::width() const
{
	return width_;
}

int16_t IntegerBlastType::signedness() const
{
	return signedness_;
}

bool IntegerBlastType::isEqual(const BlastType &otherType) const
{
	auto pOtherType = dynamic_cast<const IntegerBlastType*>(&otherType);
	return width() == pOtherType->width() && signedness() == pOtherType->signedness();
}

FloatBlastType::FloatBlastType(int16_t width) :
	BasicBlastType(EBlastType::Float), width_(width)
{
}

int16_t FloatBlastType::width() const
{
	return width_;
}

bool FloatBlastType::isEqual(const BlastType &otherType) const
{
	auto pOtherType = dynamic_cast<const FloatBlastType*>(&otherType);
	return width() == pOtherType->width();
}
