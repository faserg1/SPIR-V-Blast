#include "preprocessor.hpp"
#include <stdexcept>
#include <algorithm>
#include <regex>

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

std::string Preprocessor::process(std::string file)
{
	auto currentPath = std::filesystem::current_path() / file;
	if (!std::filesystem::exists(currentPath) || std::filesystem::is_directory(currentPath) || std::filesystem::is_empty(currentPath))
		throw std::runtime_error("File \"" + file + "\" is invalid!");
	if (!loadCallback_ || !searchCallback_)
		throw std::runtime_error("Load or search callbacks are invalid!");
	auto text = loadCallback_(currentPath);
	auto definesCopy = defines_;
	return recursiveParse(std::filesystem::current_path(), definesCopy, std::move(text));
}

std::string Preprocessor::recursiveParse(std::filesystem::path currentFolder, std::unordered_map<std::string, std::string> &defines, std::string text)
{
	if (text.empty())
		return std::move(text);
	auto removeDiscardAfter = std::remove(text.begin(), text.end(), '\r');
	text.erase(removeDiscardAfter, text.end());
	text = removeWhitespaces(std::move(text));

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

	// TODO: Parse rows

	for (auto &row : rows)
	{
		if (row[0] == '#')
			text += parsePreprocessorCommand(currentFolder, row, defines);
		else
			text += replaceByPreprocessorDefines(row, defines) + "\n";
	}

	return std::move(text);
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

std::string Preprocessor::parsePreprocessorCommand(std::filesystem::path currentFolder, std::string text, std::unordered_map<std::string, std::string> &defines)
{
	std::sregex_iterator end;

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
