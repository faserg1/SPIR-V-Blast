#include "../gen/blast_scanner.hpp"
#include <memory>

std::shared_ptr<gen::BlastScanner> buildScanner()
{
	return std::make_shared<gen::BlastScanner>();
}