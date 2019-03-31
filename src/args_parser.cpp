#include "args_parser.hpp"
#include <stdexcept>

void ArgsParser::setArgsAndParse(std::vector<std::string> args)
{
	args_ = std::move(args);
	// remove first element - the name of executable
	args_.erase(args_.begin());
	parse();
	validateOptions();
}

std::string ArgsParser::getInputFile()
{
	return inputFile_;
}

std::vector<Option> ArgsParser::getOptions()
{
	return options_;
}

void ArgsParser::parse()
{
	options_.clear();
	inputFile_ = {};
	int optionArgsLeft = 0;
	Option *lastOption = nullptr;

	std::vector<std::string> generalArguments;
	int generalArgumentsLeft = 1;

	for (auto arg : args_)
	{
		if (arg[0] == '-')
		{
			if (optionArgsLeft > 0)
			{
				throw std::runtime_error("Not all arguments for option \"" + lastOption->name + "\"");
			}
			options_.push_back({arg});
			lastOption = &options_.back();
			optionArgsLeft = getOptionArgsCount(arg);
		}
		else if (optionArgsLeft > 0)
		{
			lastOption->optionArgs.push_back(arg);
			optionArgsLeft--;
		}
		else
		{
			if (!generalArgumentsLeft)
			{
				throw std::runtime_error("Ivalid usage!");
			}
			generalArguments.push_back(arg);
			generalArgumentsLeft--;
		}
	}

	inputFile_ = generalArguments[0];
}

void ArgsParser::validateOptions()
{
	for (auto &option : options_)
	{
		if (!validateOption(option.name))
			throw std::runtime_error("Ivalid option \"" + option.name + "\"");
	}
}

bool ArgsParser::validateOption(const std::string &arg)
{
	if (arg == "-o" || arg == "--output")
		return true;
	if (arg == "-D" || arg == "--define")
		return true;
	return false;
}

int ArgsParser::getOptionArgsCount(const std::string &option)
{
	if (option == "-o" || option == "--output")
		return 1;
	if (option == "-D" || option == "--define")
		return 1;
	return 0;
}