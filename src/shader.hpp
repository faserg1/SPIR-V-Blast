#ifndef SHADER
#define SHADER

#include <string>
#include <spirv.hpp11>
#include <vector>


enum class EShaderOperationArgumentType
{
	Invalid,
	Literal,
	StringLiteral
};

struct ShaderOperationArgument
{
	union
	{
		std::string literal;
		std::string stringLiteral;
	} u;
	EShaderOperationArgumentType type;
};

struct ShaderOperation
{
	std::string optInstructionTypeId;
	std::string optResultId;
	spv::Op operation;
	std::vector<ShaderOperationArgument> arguments;
};

class Shader
{
public:
private:

};

#endif