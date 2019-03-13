#include "args.hpp"
#include <stdexcept>

void Args::setArgsAndParse(std::vector<std::string> args)
{
	args_ = std::move(args);
	// remove first element - the name of executable
	args_.erase(args_.begin());
	parse();
	validateOptions();
}

std::string Args::getInputFile()
{
	return inputFile_;
}

std::vector<Option> Args::getOptions()
{
	return options_;
}

void Args::parse()
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

void Args::validateOptions()
{
	for (auto &option : options_)
	{
		if (!validateOption(option.name))
			throw std::runtime_error("Ivalid option \"" + option.name + "\"");
	}
}

bool Args::validateOption(const std::string &arg)
{
	if (arg == "-o" || arg == "--output")
		return true;
	return false;
}

int Args::getOptionArgsCount(const std::string &option)
{
	if (option == "-o" || option == "--output")
		return 1;
	return 0;
}