#ifndef COMPILER_TYPES_PARSER
#define COMPILER_TYPES_PARSER

#include <string>
#include <vector>
#include "compiler_parser.hpp"
#include "compiler_types.hpp"

class CompilerBasicTypeParser :
	public CompilerParser
{
public:
	CompilerBasicTypeParser();

	bool tryVisit(const std::string &expression) override;
	bool next() override;
	std::shared_ptr<CompilerNode> end() override;
private:
	struct
	{
		std::string type_;
		bool parametersRecord_;
		bool separatorAwait_;
		bool recordEnd_;
		std::vector<std::string> parameters_;
	} data;
private:
	bool isType(std::string expression) const;
};

#endif