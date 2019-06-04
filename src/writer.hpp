#ifndef WRITER
#define WRITER

#include <cstdint>
#include <string>
#include <vector>
#include <variant>

class Writer
{
public:
	void setOutput(std::string output);
	std::string getOutput() const;
	void write(std::variant<std::string, std::vector<uint32_t>> text);
private:
	std::string output_;
private:
	void writeText(std::string text);
	void writeBinary(std::vector<uint32_t> binary);
};

#endif // WRITER