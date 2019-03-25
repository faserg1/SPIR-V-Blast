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
	preprocessedInfo_.text_ = recursiveParse(std::filesystem::current_path(), definesCopy, std::move(text));
	return preprocessedInfo_;
}

std::string Preprocessor::recursiveParse(std::filesystem::path currentFolder, std::unordered_map<std::string, std::string> &defines, std::string text)
{
	if (text.empty())
		return std::move(text);
	auto removeDiscardAfter = std::remove(text.begin(), text.end(), '\r');
	text.erase(removeDiscardAfter, text.end());
	text = removeWhitespaces(std::move(text));
	text = removeComments(std::move(text));

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
		
		rows.push_back(removeWhitespaces(std::move(row)));
	}

	PreprocessorState state;

	for (auto &row : rows)
	{
		if (row[0] == '#')
			text += parsePreprocessorCommand(currentFolder, row, defines, state);
		else if (!state.ignoreText)
			text += replaceByPreprocessorDefines(row, defines) + "\n";
	}

	return removeWhitespaces(std::move(text));
}

std::string Preprocessor::removeWhitespaces(std::string text)
{
	std::string textCopy;
	textCopy.reserve(text.size());

	bool symbolCopy = false;
	bool space = false; 

	for (auto &c : text)
	{
		switch (c)
		{
		case '\t':
		case ' ':
			if (!symbolCopy)
			{
				if (space)
					continue;
				space = true;
				break;
			}
			else
			{
				textCopy.push_back(c);
				break;
			}
		case '\"':
		case '\'':
			symbolCopy = !symbolCopy;
		default:
			if (space && (!textCopy.empty() && textCopy.back() != '\n') && c != '\n')
				textCopy.push_back(' ');
			space = false;
			if (c == '\n' && (textCopy.empty() || textCopy.back() == '\n'))
				continue;
			else
				textCopy.push_back(c);
			break;
		}
	}
	textCopy.shrink_to_fit();
	return std::move(textCopy);
}

std::string Preprocessor::removeComments(std::string text)
{
	std::sregex_iterator end;
	while (true)
	{
		std::regex multilineCommentRegex("\\/\\*(.|\\n)*\\*\\/", std::regex_constants::ECMAScript | std::regex_constants::optimize);
		auto multilineCommentMatchIter = std::sregex_iterator(text.begin(), text.end(), multilineCommentRegex);
		if (multilineCommentMatchIter != end && !(*multilineCommentMatchIter).empty())
		{
			auto match = *multilineCommentMatchIter;
			text.erase(text.begin() + match.position(), text.begin() + match.position() + match.length());
			continue;
		}

		std::regex inlineCommentRegex("\\/\\/.*\n", std::regex_constants::ECMAScript);
		auto inlineCommentMatchIter = std::sregex_iterator(text.begin(), text.end(), inlineCommentRegex);
		if (inlineCommentMatchIter != end && !(*inlineCommentMatchIter).empty())
		{
			auto match = *inlineCommentMatchIter;
			auto lineCommentEnd = text.erase(text.begin() + match.position(), text.begin() + match.position() + match.length());
			text.insert(lineCommentEnd, '\n');
			continue;
		}

		break;
	}
	
	return std::move(text);
}

std::string Preprocessor::parsePreprocessorCommand(std::filesystem::path currentFolder, std::string text,
	std::unordered_map<std::string, std::string> &defines, PreprocessorState &state)
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
		return recursiveParse(filepath.parent_path(), defines, std::move(includeText));
	}

	std::regex includeGlobalRegex("^#include\\s+\\<([\\w\\.\\d_\\-\\/\\\\]+)\\>$", std::regex_constants::ECMAScript);
	auto includeGlobalMatchIter = std::sregex_iterator(text.begin(), text.end(), includeGlobalRegex);
	if (includeGlobalMatchIter != end && !(*includeGlobalMatchIter).empty())
	{
		auto match = *includeGlobalMatchIter;
		std::string filename = match[1];
		auto filepath = searchCallback_(currentFolder, filename, false);
		auto includeText = loadCallback_(filepath);
		return recursiveParse(filepath.parent_path(), defines, std::move(includeText));
	}

	// Meta-data

	std::regex entryPointRegex("^#entry\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)$", std::regex_constants::ECMAScript);
	auto entryPointMatchIter = std::sregex_iterator(text.begin(), text.end(), entryPointRegex);
	if (entryPointMatchIter != end && !(*entryPointMatchIter).empty())
	{
		auto match = *entryPointMatchIter;
		preprocessedInfo_.addEntryPoint(match[1], match[2], match[3]);
		return std::string();
	}

	std::regex memoryModelRegex("^#memory\\s+(\\w+)\\s+(\\w+)$", std::regex_constants::ECMAScript);
	auto memoryModelMatchIter = std::sregex_iterator(text.begin(), text.end(), memoryModelRegex);
	if (memoryModelMatchIter != end && !(*memoryModelMatchIter).empty())
	{
		auto match = *memoryModelMatchIter;
		preprocessedInfo_.setMemoryModel(match[1], match[2]);
		return std::string();
	}

	return std::string();
}

std::string Preprocessor::replaceByPreprocessorDefines(std::string text, const std::unordered_map<std::string, std::string> &defines)
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
	int64_t offset = 0;
	for (auto &rep : reps)
	{
		auto where = text.erase(rep.begin + offset, rep.begin + offset + rep.count);
		offset += ((int64_t) rep.replacer.size()) - ((int64_t) rep.count);
		text.insert(where, rep.replacer.begin(), rep.replacer.end());
	}
	return std::move(text);
}
