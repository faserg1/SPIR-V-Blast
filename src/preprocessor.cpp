#include "preprocessor.hpp"
#include <stdexcept>

void Preprocessor::addDefine(std::string key, std::string value)
{
	defines_.insert(std::make_pair(key, value));
}

void Preprocessor::setSearchFileCallback(searchCallback searchCallback)
{
	searchCallback_ = searchCallback;
}

void Preprocessor::setLoadCallback(loadCallback loadCallback)
{
	loadCallback_ = loadCallback;
}

void Preprocessor::startProcess(std::string file)
{
	auto currentPath = std::filesystem::current_path() / file;
	if (!std::filesystem::exists(currentPath) || std::filesystem::is_directory(currentPath) || std::filesystem::is_empty(currentPath))
		throw std::runtime_error("File \"" + file + "\" is invalid!");
	if (!loadCallback_ || !searchCallback_)
		throw std::runtime_error("Load or search callbacks are invalid!");
	auto text = loadCallback_(currentPath);
}
