#ifndef PREPROCESSOR
#define PREPROCESSOR

#include <string>
#include <unordered_map>
#include <functional>
#include <filesystem>

class Preprocessor
{
public:
	using searchCallback = std::function<std::filesystem::path (std::filesystem::path currentFolder, std::string filename, bool local)>;
	using loadCallback = std::function<std::string(std::filesystem::path)>;

	void addDefine(std::string key, std::string value = "");
	void setSearchFileCallback(searchCallback searchCallback);
	void setLoadCallback(loadCallback loadCallback);
	void startProcess(std::string file);
private:
	std::unordered_map<std::string, std::string> defines_;
	searchCallback searchCallback_;
	loadCallback loadCallback_;
};

#endif