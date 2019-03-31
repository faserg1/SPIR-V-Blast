#ifndef PARSER_SYMBOLS
#define PARSER_SYMBOLS

#include <string>
#include <variant>
#include <memory>

enum class EBlastSymbolType
{
	Name,
	StringLiteral,
	CharacterLiteral,
	NumberLiteral,
};

class BlastSymbol :
	public std::enable_shared_from_this<BlastSymbol>
{
protected:
	BlastSymbol(EBlastSymbolType type);

	EBlastSymbolType getType() const;
private:
	const EBlastSymbolType type_;
};

class BlastSymbolName :
	public BlastSymbol
{
public:
	BlastSymbolName(std::string name);

	std::string getName() const;
private:
	const std::string name_;
};

class BlastSymbolString :
	public BlastSymbol
{
public:
	BlastSymbolString(std::string string);

	std::string getString() const;
private:
	const std::string string_;
};

class BlastSymbolCharacter :
	public BlastSymbol
{
public:
	BlastSymbolCharacter(char character);

	char getCharacter() const;
private:
	char character_;
};

class BlastSymbolNumber :
	public BlastSymbol
{
public:
	using number_t = std::variant<unsigned long, long, double>;
	BlastSymbolNumber(number_t number);

	number_t getNumber() const;
private:
	const number_t number_;
};

#endif