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
	std::string process(std::string file);
private /*members*/:
	std::unordered_map<std::string, std::string> defines_;
	searchCallback searchCallback_;
	loadCallback loadCallback_;

	struct PreprocessorState;
private /*functions*/:
	std::string recursiveParse(std::filesystem::path currentFolder, std::unordered_map<std::string, std::string> &defines, std::string inputText);
	std::string removeWhitespaces(std::string text);
	std::string removeComments(std::string text);
	std::string parsePreprocessorCommand(std::filesystem::path currentFolder, std::string text,
		std::unordered_map<std::string, std::string> &defines, PreprocessorState &state);
	std::string replaceByPreprocessorDefines(std::string text, const std::unordered_map<std::string, std::string> &defines);
};

#endif