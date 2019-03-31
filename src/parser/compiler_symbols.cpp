#include "compiler_symbols.hpp"

BlastSymbol::BlastSymbol(EBlastSymbolType type) :
	type_(type)
{
	
}

EBlastSymbolType BlastSymbol::getType() const
{
	return type_;
}

BlastSymbolName::BlastSymbolName(std::string name) :
	BlastSymbol(EBlastSymbolType::Name), name_(name)
{
}

std::string BlastSymbolName::getName() const
{
	return name_;
}

BlastSymbolString::BlastSymbolString(std::string string) :
	BlastSymbol(EBlastSymbolType::StringLiteral), string_(string)
{
}

std::string BlastSymbolString::getString() const
{
	return string_;
}

BlastSymbolCharacter::BlastSymbolCharacter(char character) :
	BlastSymbol(EBlastSymbolType::CharacterLiteral), character_(character)
{
}

char BlastSymbolCharacter::getCharacter() const
{
	return character_;
}

BlastSymbolNumber::BlastSymbolNumber(number_t number) :
	BlastSymbol(EBlastSymbolType::NumberLiteral), number_(number)
{
}

BlastSymbolNumber::number_t BlastSymbolNumber::getNumber() const
{
	return number_;
}
