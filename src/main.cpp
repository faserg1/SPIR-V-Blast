#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "args_parser.hpp"
#include "args_processor.hpp"
#include "preprocessor.hpp"
#include "reader.hpp"
#include "parser/parser.hpp"
#include "compiler/compiler.hpp"
#include "translator/translator.hpp"
#include "writer.hpp"

void printHelp();
void printUsage();

int main(int argc, char**argv)
{
	std::vector<std::string> args(argc);
	for (int i = 0; i < argc; i++)
		args[i] = std::string(argv[i]);
	ArgsParser argsParser;
	try
	{
		argsParser.setArgsAndParse(args);
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
		return 1;
	}

	auto options = argsParser.getOptions();

	Preprocessor preprocessor;
	Reader reader;
	Parser parser;
	Compiler compiler;
	Translator translator;
	Writer writer;

	processPreprocessorFlags(preprocessor, options);
	processReaderFlags(reader, options);

	preprocessor.setLoadCallback(std::bind(&Reader::read, &reader, std::placeholders::_1));
	preprocessor.setSearchFileCallback(std::bind(&Reader::search, &reader, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	auto processedInfo = preprocessor.process(argsParser.getInputFile());
	auto nodes = parser.parse(processedInfo);
	compiler.compile(processedInfo, nodes);

	return 0;
}

void printHelp()
{
	printUsage();
}

void printUsage()
{
	std::cout << "Blast [options] <input file>" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "\t-o|--output <output file> \t- set the outputfile" << std::endl;
	std::cout << "\t-D|--define <KEY=VALUE> \t- add the define for preprocessor" << std::endl;
	std::cout << "\t-I|--include <include dir> \t- add the include directory for file search" << std::endl;
}