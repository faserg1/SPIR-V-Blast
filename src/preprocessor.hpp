#ifndef PREPROCESSOR
#define PREPROCESSOR

#include <string>
#include <unordered_map>
#include <functional>

class Preprocessor
{
public:
	void addSource(std::string filename, std::string text);
	void addDefine(std::string key, std::string value = "");
	void setSearchFileCallback(std::function<bool /*searchFile*/(std::string /*filename*/)> searchCallback);
private:
	std::unordered_map<std::string, std::string> defines_;
	std::unordered_map<std::string, std::string> sources_;
	std::function<bool /*searchFile*/(std::string /*filename*/)> searchCallback_;
};

#endif