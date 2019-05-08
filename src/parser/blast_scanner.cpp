#include "../gen/blast_scanner.hpp"
#include <memory>

std::shared_ptr<gen::BlastScanner> buildScanner(const std::string &source)
{
	auto scanner = std::make_shared<gen::BlastScanner>(source);
	return scanner;
}