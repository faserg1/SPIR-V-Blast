#include "preprocessor.hpp"
#include <stdexcept>
#include <algorithm>
#include <regex>

struct Preprocessor::PreprocessorState
{
	bool ignoreText = false;
	size_t ignoreTextLevel = 0;
	size_t ifdefLevel = 0;
};

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

ShaderPreprocessedInfo Preprocessor::process(std::string file)
{
	auto currentPath = std::filesystem::current_path() / file;
	if (!std::filesystem::exists(currentPath) || std::filesystem::is_directory(currentPath) || std::filesystem::is_empty(currentPath))
		throw std::runtime_error("File \"" + file + "\" is invalid!");
	if (!loadCallback_ || !searchCallback_)
		throw std::runtime_error("Load or search callbacks are invalid!");
	auto text = loadCallback_(currentPath);
	auto definesCopy = defines_;
	preprocessedInfo_.text_ = recursiveParse(std::filesystem::current_path(), definesCopy, std::move(text), currentPath.string(), preprocessedInfo_.debugRowsInfo_);
	preprocessedInfo_.sourceFiles_ = std::move(sourceFiles_);
	return preprocessedInfo_;
}

std::string Preprocessor::recursiveParse(std::filesystem::path currentFolder, defines_t &defines, std::string text, std::string filePath, std::vector<DebugRowInfo> &debugRowsInfo)
{
	if (text.empty())
		return std::move(text);
	
	size_t sourceId = sourceFiles_.size();
	sourceFiles_.push_back(filePath);
	std::vector<DebugRowInfo> localDebugRowsInfo;
	auto removeDiscardAfter = std::remove(text.begin(), text.end(), '\r');
	text.erase(removeDiscardAfter, text.end());
	{
		auto linesCount = std::count(text.begin(), text.end(), '\n') + 1;
		size_t lineNum = 0;
		localDebugRowsInfo.resize(linesCount, {sourceId, 0});
		std::for_each(localDebugRowsInfo.begin(), localDebugRowsInfo.end(), [&lineNum](DebugRowInfo &rowInfo) -> void
		{
			rowInfo.sourceRowNumber = lineNum++;
		});
	}

	std::vector<std::string> rows;
	while (!text.empty())
	{
		auto rowDelimIt = text.begin();
		while (true)
		{
			rowDelimIt = std::find(rowDelimIt, text.end(), '\n');
			if (rowDelimIt == text.end())
				break;
			if (text[0] == '#' && *(rowDelimIt - 1) == '\\')
			{
				rowDelimIt++;
				continue;
			}
			break;
		}
		auto stop = static_cast<size_t>(std::distance(text.begin(), rowDelimIt));
		if (stop > text.size())
			stop = text.size();
		auto row = text.substr(0, stop);
		text.erase(0, (text.size() == stop ? stop : stop+1));

		do
		{
			std::string slashN = "\\\n";
			auto itSlashN = std::find_first_of(row.begin(), row.end(), slashN.begin(), slashN.end());
			if (itSlashN == row.end())
				break;
			if (std::distance(itSlashN, row.end()) <= 2)
				row.erase(itSlashN, itSlashN + 1);
			else
				row.erase(itSlashN, itSlashN + 2);
		} while (true);
		
		rows.push_back(std::move(row));
	}

	PreprocessorState state;
	std::vector<DebugRowInfo> localProcessedDebugRowsInfo;
	
	{
		auto rowInfo = localDebugRowsInfo.begin();
		for (auto &row : rows)
		{
			if (row[0] == '#')
				text += parsePreprocessorCommand(currentFolder, row, defines, state, localProcessedDebugRowsInfo);
			else if (!state.ignoreText)
			{
				auto processedText = replaceByPreprocessorDefines(row, defines) + "\n";
				text += processedText;
				auto newLines = std::count(processedText.begin(), processedText.end(), '\n');
				do
				{
					localProcessedDebugRowsInfo.push_back(*rowInfo);
				} while (--newLines);
			}
			rowInfo++;
		}
		//remove last \n 
		text.erase(text.end() - 1);
	}

	debugRowsInfo.insert(debugRowsInfo.end(), localProcessedDebugRowsInfo.begin(), localProcessedDebugRowsInfo.end());
	return std::move(text);
}

std::string Preprocessor::parsePreprocessorCommand(std::filesystem::path currentFolder, std::string text, defines_t &defines,
	PreprocessorState &state, std::vector<DebugRowInfo> &debugRowsInfo)
{
	std::sregex_iterator end;

	std::regex endifRegex("^#endif$", std::regex_constants::ECMAScript);
	auto endifMatchIter = std::sregex_iterator(text.begin(), text.end(), endifRegex);
	if (endifMatchIter != end && !(*endifMatchIter).empty())
	{
		auto match = *endifMatchIter;
		if (state.ifdefLevel == state.ignoreTextLevel)
		{
			state.ignoreText = false;
		}
		state.ifdefLevel--;
		return std::string();
	}

	std::regex ifdefRegex("^#ifdef\\s+(\\w+)$", std::regex_constants::ECMAScript);
	auto ifdefMatchIter = std::sregex_iterator(text.begin(), text.end(), ifdefRegex);
	if (ifdefMatchIter != end && !(*ifdefMatchIter).empty())
	{
		auto match = *ifdefMatchIter;
		std::string key = match[1];
		state.ifdefLevel++;
		if (!state.ignoreText && defines.find(key) == defines.end())
		{
			state.ignoreTextLevel = state.ifdefLevel;
			state.ignoreText = true;
		}
		return std::string();
	}

	std::regex ifndefRegex("^#ifndef\\s+(\\w+)$", std::regex_constants::ECMAScript);
	auto ifndefMatchIter = std::sregex_iterator(text.begin(), text.end(), ifndefRegex);
	if (ifndefMatchIter != end && !(*ifndefMatchIter).empty())
	{
		auto match = *ifndefMatchIter;
		std::string key = match[1];
		state.ifdefLevel++;
		if (!state.ignoreText && defines.find(key) != defines.end())
		{
			state.ignoreTextLevel = state.ifdefLevel;
			state.ignoreText = true;
		}
		return std::string();
	}

	if (state.ignoreText)
		return std::string();

	std::regex defineRegex("^#define\\s+(\\w+)(\\s+(\\w+|\".*\"|\'.*\'))?$", std::regex_constants::ECMAScript);
	auto defineMatchIter = std::sregex_iterator(text.begin(), text.end(), defineRegex);
	if (defineMatchIter != end && !(*defineMatchIter).empty())
	{
		auto match = *defineMatchIter;
		std::string key = match[1];
		std::string value;
		if (match.size() > 2)
			value = replaceByPreprocessorDefines(match[3], defines);
		if (defines.find(key) == defines.end())
			defines.insert(std::make_pair(key, value));
		else
			defines[key] = value;
		return std::string();
	}

	std::regex undefRegex("^#undef\\s+(\\w+)$", std::regex_constants::ECMAScript);
	auto undefMatchIter = std::sregex_iterator(text.begin(), text.end(), undefRegex);
	if (undefMatchIter != end && !(*undefMatchIter).empty())
	{
		auto match = *undefMatchIter;
		std::string key = match[1];
		defines.erase(key);
		return std::string();
	}

	std::regex includeLocalRegex("^#include\\s+\\\"([\\w\\.\\d_\\-\\/\\\\]+)\\\"$", std::regex_constants::ECMAScript);
	auto includeLocalMatchIter = std::sregex_iterator(text.begin(), text.end(), includeLocalRegex);
	if (includeLocalMatchIter != end && !(*includeLocalMatchIter).empty())
	{
		auto match = *includeLocalMatchIter;
		std::string filename = match[1];
		auto filepath = searchCallback_(currentFolder, filename, true);
		auto includeText = loadCallback_(filepath);
		return recursiveParse(filepath.parent_path(), defines, std::move(includeText), filepath.string(), debugRowsInfo);
	}

	std::regex includeGlobalRegex("^#include\\s+\\<([\\w\\.\\d_\\-\\/\\\\]+)\\>$", std::regex_constants::ECMAScript);
	auto includeGlobalMatchIter = std::sregex_iterator(text.begin(), text.end(), includeGlobalRegex);
	if (includeGlobalMatchIter != end && !(*includeGlobalMatchIter).empty())
	{
		auto match = *includeGlobalMatchIter;
		std::string filename = match[1];
		auto filepath = searchCallback_(currentFolder, filename, false);
		auto includeText = loadCallback_(filepath);
		return recursiveParse(filepath.parent_path(), defines, std::move(includeText), filepath.string(), debugRowsInfo);
	}

	// Meta-data

	std::regex memoryModelRegex("^#memory\\s+(\\w+)\\s+(\\w+)$", std::regex_constants::ECMAScript);
	auto memoryModelMatchIter = std::sregex_iterator(text.begin(), text.end(), memoryModelRegex);
	if (memoryModelMatchIter != end && !(*memoryModelMatchIter).empty())
	{
		auto match = *memoryModelMatchIter;
		preprocessedInfo_.setMemoryModel(match[1], match[2]);
		return std::string();
	}

	std::regex capabilityRegex("^#capability\\s+(\\w+)$", std::regex_constants::ECMAScript);
	auto capabilityMatchIter = std::sregex_iterator(text.begin(), text.end(), capabilityRegex);
	if (capabilityMatchIter != end && !(*capabilityMatchIter).empty())
	{
		auto match = *capabilityMatchIter;
		preprocessedInfo_.addCapability(match[1]);
		return std::string();
	}

	return std::string();
}

std::string Preprocessor::replaceByPreprocessorDefines(std::string text, const defines_t &defines)
{
	struct Replaces
	{
		std::string::iterator begin;
		size_t count;
		std::string replacer;
	};
	std::vector<Replaces> reps;
	for (const auto &define : defines)
	{
		std::regex self_regex("\\b" + define.first + "\\b", std::regex_constants::ECMAScript);
		auto replaces = std::sregex_iterator(text.begin(), text.end(), self_regex);
		auto replacesEnd = std::sregex_iterator();
		for (auto i = replaces; i != replacesEnd; ++i)
		{
			auto match = *i;
			auto beg = text.begin() + match.position();
			size_t len = match.length();
			reps.push_back(
				{
					beg,
					len,
					define.second,
				});
		}
	}
	if (reps.empty())
		return std::move(text);
	std::sort(reps.begin(), reps.end(), [](const Replaces &r1, const Replaces &r2) -> bool
	{
		return r1.begin > r2.begin;
	});
	for (auto &rep : reps)
	{
		auto where = text.erase(rep.begin, rep.begin + rep.count);
		text.insert(where, rep.replacer.begin(), rep.replacer.end());
	}
	return std::move(text);
}
