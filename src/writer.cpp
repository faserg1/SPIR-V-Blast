#include "writer.hpp"
#include <fstream>
#include <stdexcept>

void Writer::setOutput(std::string output)
{
	output_ = output;
}

std::string Writer::getOutput() const
{
	return output_;
}

void Writer::write(std::variant<std::string, std::vector<uint32_t>> text)
{
	if (std::holds_alternative<std::string>(text))
		writeText(std::get<std::string>(text));
	else
		writeBinary(std::get<std::vector<uint32_t>>(text));
}

void Writer::writeText(std::string text)
{
	std::ofstream output(output_, std::ios::out);
	output << text;
}

void Writer::writeBinary(std::vector<uint32_t> binary)
{
	std::ofstream output(output_,std::ios::binary | std::ios::out);
	for (auto word : binary)
	{
		auto wordBytes = reinterpret_cast<char *>(&word);
		output.write(wordBytes, sizeof(uint32_t));
	}
}
