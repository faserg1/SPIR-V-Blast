#ifndef PARSER_TYPES_PARSER
#define PARSER_TYPES_PARSER

#include <string>
#include <vector>
#include "common_parser.hpp"
#include "parser_types.hpp"

class BasicBlastTypeParser :
	public CommonParser
{
public:
	BasicBlastTypeParser();

	bool tryVisit(const std::string &expression) override;
	bool next() override;
	std::shared_ptr<ParserNode> end(EParserState state) override;
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