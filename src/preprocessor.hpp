#ifndef PREPROCESSOR
#define PREPROCESSOR

#include <string>
#include <unordered_map>
#include <functional>
#include <filesystem>

#include "shader_preprocessed_info.hpp"

class Preprocessor
{
	using defines_t = std::unordered_map<std::string, std::string>;
public:
	using searchCallback = std::function<std::filesystem::path (std::filesystem::path currentFolder, std::string filename, bool local)>;
	using loadCallback = std::function<std::string(std::filesystem::path)>;

	void addDefine(std::string key, std::string value = "");
	void setSearchFileCallback(searchCallback searchCallback);
	void setLoadCallback(loadCallback loadCallback);
	ShaderPreprocessedInfo process(std::string file);
private /*members*/:
	defines_t defines_;
	std::vector<std::string> sourceFiles_;
	searchCallback searchCallback_;
	loadCallback loadCallback_;
	ShaderPreprocessedInfo preprocessedInfo_;

	struct PreprocessorState;
private /*functions*/:
	std::string recursiveParse(std::filesystem::path currentFolder, defines_t &defines, std::string inputText, std::string filePath, std::vector<DebugRowInfo> &debugRowsInfo);
	std::string removeWhitespaces(std::string text, std::vector<DebugRowInfo> &debugRowInfo);
	std::string removeComments(std::string text, std::vector<DebugRowInfo> &debugRowInfo);
	std::string parsePreprocessorCommand(std::filesystem::path currentFolder, std::string text, defines_t &defines, PreprocessorState &state, std::vector<DebugRowInfo> &debugRowsInfo);
	std::string replaceByPreprocessorDefines(std::string text, const defines_t &defines);
};

#endif