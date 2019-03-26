#include "compiler.hpp"
#include <algorithm>
#include <regex>

Shader Compiler::compile(const ShaderPreprocessedInfo &preprocessedInfo)
{
	auto literals = splitByLiterals(std::move(preprocessedInfo.text()));

	return Shader();
}

std::vector<std::string> Compiler::splitByLiterals(std::string text)
{
	auto separatorsCount = std::count_if(text.begin(), text.end(),
		[](char c) -> bool {return c == ' ' || c == '\t' || c == '\n'; });
	std::vector<std::string> preliterals;
	preliterals.reserve(separatorsCount + 1);

	while (!text.empty())
	{
		bool escape = false;
		bool stringCapture = false;
		bool literalCapture = false;
		auto iterSep = std::find_if(text.begin(), text.end(),
			[&escape, &stringCapture, &literalCapture](char c) -> bool
		{
			bool wasEscape = escape;
			escape = false;
			switch (c)
			{
			case '\\':
				if (stringCapture || literalCapture)
					escape = true;
				return false;
			case '\"':
				if (!escape && !literalCapture)
					stringCapture = !stringCapture;
				return false;
			case '\'':
				if (!escape && !stringCapture)
					literalCapture = !literalCapture;
				return false;
			case ' ':
			case '\t':
				if (stringCapture || literalCapture)
					return false;
			case '\n':
				return true;
			default:
				return false;
			}
		});
		auto distance = std::distance(text.begin(), iterSep);
		auto substr = text.substr(0, distance);
		preliterals.push_back(substr);
		text.erase(0, distance+1);
	}

	std::string wordsAndLiterals = "(\\\"((\\\\\\\")|[^\"])*\\\")|(\\\'.\\\')|(\\b\\w+\\b)";
	std::string signs = "(\\<\\=)|(\\>\\=)|(\\=\\=)|(\\!\\=)|(\\=)|(\\!)|(\\+)|(\\-)|(\\*)|(\\\\)";
	std::string brackets = "(\\{)|(\\})|(\\()|(\\))|(\\[)|(\\])";
	std::string other = "(;)|(\\?)";
	std::string expression = wordsAndLiterals + "|" + signs + "|" + brackets + "|" + other;

	std::regex literalRegex(expression, std::regex_constants::ECMAScript | std::regex_constants::optimize);
	std::sregex_iterator end;

	std::vector <std::string> literals;
	literals.reserve(preliterals.size() * 2);

	for (auto iter = preliterals.begin(); iter != preliterals.end(); iter++)
	{
		auto txt = *iter;
		auto literalMatchIter = std::sregex_iterator(txt.begin(), txt.end(), literalRegex);
		if (literalMatchIter == end)
			throw std::runtime_error("Unknown literal!");
		for (; literalMatchIter != end; literalMatchIter++)
		{
			auto match = *literalMatchIter;
			for (auto matchIter = match.begin() + 1; matchIter < match.end(); matchIter++)
			{
				if (!matchIter->matched)
					continue;
				auto value = *matchIter;
				literals.push_back(value);
				break;
			}
		}
	};

	return literals;
}
