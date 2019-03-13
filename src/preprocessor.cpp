#include "preprocessor.hpp"

void Preprocessor::addSource(std::string filename, std::string text)
{
	sources_.insert(std::make_pair(filename, text));
}

void Preprocessor::addDefine(std::string key, std::string value)
{
	defines_.insert(std::make_pair(key, value));
}

void Preprocessor::setSearchFileCallback(std::function<bool(std::string)> searchCallback)
{
	searchCallback_ = searchCallback;
}
