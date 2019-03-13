#include <iostream>
#include <vector>
#include <string>

void printHelp();
void printUsage();

int main(int argc, char**argv)
{
	std::vector<std::string> args(argc);
	for (int i = 0; i < argc; i++)
		args[i] = std::string(argv[i]);
	
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
	std::cout << "\t-o <output file> \t- set the outputfile" << std::endl;
}