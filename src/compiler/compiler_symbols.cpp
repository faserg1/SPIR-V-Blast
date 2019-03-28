#include "compiler_symbols.hpp"

CompilerSymbol::CompilerSymbol(ECompilerSymbolType type) :
	type_(type)
{
	
}

ECompilerSymbolType CompilerSymbol::getType() const
{
	return type_;
}

CompilerSymbolName::CompilerSymbolName(std::string name) :
	CompilerSymbol(ECompilerSymbolType::Name), name_(name)
{
}

std::string CompilerSymbolName::getName() const
{
	return name_;
}

CompilerSymbolString::CompilerSymbolString(std::string string) :
	CompilerSymbol(ECompilerSymbolType::StringLiteral), string_(string)
{
}

std::string CompilerSymbolString::getString() const
{
	return string_;
}

CompilerSymbolCharacter::CompilerSymbolCharacter(char character) :
	CompilerSymbol(ECompilerSymbolType::CharacterLiteral), character_(character)
{
}

char CompilerSymbolCharacter::getCharacter() const
{
	return character_;
}

CompilerSymbolNumber::CompilerSymbolNumber(number_t number) :
	CompilerSymbol(ECompilerSymbolType::NumberLiteral), number_(number)
{
}

CompilerSymbolNumber::number_t CompilerSymbolNumber::getNumber() const
{
	return number_;
}
