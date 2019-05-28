#ifndef COMPILER_DATA
#define COMPILER_DATA

#include <vector>
#include <string>
#include <memory>
#include <any>
#include <cstdint>
#include <spirv.hpp11>

struct Id
{
	uint32_t id;
	std::string debugName;
};

#endif // COMPILER_DATA