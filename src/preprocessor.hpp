#ifndef PREPROCESSOR
#define PREPROCESSOR

#include <string>
#include <unordered_map>

class Preprocessor
{
public:
	void addSource(std::string filename, std::string text);
	void addDefine(std::string key, std::string value = "");
private:
	std::unordered_map<std::string, std::string> defines_;
	std::unordered_map<std::string, std::string> sources_;
};

#endif