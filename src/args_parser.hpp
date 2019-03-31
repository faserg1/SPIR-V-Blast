#ifndef ARGS_PARSER
#define ARGS_PARSER

#include <vector>
#include <string>

struct Option
{
	std::string name;
	std::vector<std::string> optionArgs;
};

class ArgsParser
{
public:
	void setArgsAndParse(std::vector<std::string> args);
	std::string getInputFile();
	std::vector<Option> getOptions();
private:
	std::vector<std::string> args_;

	std::vector<Option> options_;
	std::string inputFile_;
private:
	void parse();
	void validateOptions();
	bool validateOption(const std::string &arg);
	int getOptionArgsCount(const std::string &option);
};

#endif