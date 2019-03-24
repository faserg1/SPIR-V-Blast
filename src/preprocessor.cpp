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
	return recursiveParse(defines_, std::move(text));
}

std::string Preprocessor::recursiveParse(std::unordered_map<std::string, std::string> defines, std::string text)
{
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
