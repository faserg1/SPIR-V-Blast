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
	std::string read(std::filesystem::path path);
	std::filesystem::path search(std::filesystem::path currentFolder, std::string filename, bool local);
private:
	std::vector<std::filesystem::path> searchPaths_;
private:
	std::string internalRead(std::filesystem::path path);
};

#endif