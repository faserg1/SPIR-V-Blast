#ifndef COMPILER_SYMBOLS
#define COMPILER_SYMBOLS

#include <string>
#include <variant>
#include <memory>

enum class ECompilerSymbolType
{
	Name,
	StringLiteral,
	CharacterLiteral,
	NumberLiteral,
};

class CompilerSymbol :
	public std::enable_shared_from_this<CompilerSymbol>
{
protected:
	CompilerSymbol(ECompilerSymbolType type);

	ECompilerSymbolType getType() const;
private:
	const ECompilerSymbolType type_;
};

class CompilerSymbolName :
	public CompilerSymbol
{
public:
	CompilerSymbolName(std::string name);

	std::string getName() const;
private:
	const std::string name_;
};

class CompilerSymbolString :
	public CompilerSymbol
{
public:
	CompilerSymbolString(std::string string);

	std::string getString() const;
private:
	const std::string string_;
};

class CompilerSymbolCharacter :
	public CompilerSymbol
{
public:
	CompilerSymbolCharacter(char character);

	char getCharacter() const;
private:
	char character_;
};

class CompilerSymbolNumber :
	public CompilerSymbol
{
public:
	using number_t = std::variant<unsigned long, long, double>;
	CompilerSymbolNumber(number_t number);

	number_t getNumber() const;
private:
	const number_t number_;
};

#endif