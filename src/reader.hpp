#ifndef READER
#define READER

#include <memory>
#include <string>

class Shader;

class Reader
{
public:
	std::shared_ptr<Shader> readFromFile(std::string path);
};

#endif