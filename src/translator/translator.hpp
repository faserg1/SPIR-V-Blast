#ifndef TRANSLATOR
#define TRANSLATOR

#include <vector>
#include <string>
#include <variant>
#include "../compiler/compiler_data.hpp"

class Translator
{
public:
	Translator();

	void setHumanReadable(bool readable = true);
	std::variant<std::string, std::vector<uint32_t>> translate(std::vector<SpirVOp> ops);
private:
	std::string translateHumanReadable(const SpirVOp &op);
	std::vector<uint32_t> translateBinary(const SpirVOp &op);
	uint32_t makeFirstWord(uint16_t wordCount, uint16_t opCode);
private:
	bool humanReadable_;
};

#endif // TRANSLATOR