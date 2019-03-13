#ifndef READER
#define READER

#include <memory>
#include <string>
#include <vector>
#include <filesystem>

class Reader :
	public std::enable_shared_from_this<Reader>
{
public:
	void addSearchPath(std::string path);
	std::string read(std::string filename);
private:
	std::vector<std::filesystem::path> searchPaths_;
private:
	std::string internalRead(std::string path);
};

#endif