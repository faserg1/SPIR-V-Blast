#include "parser_common.hpp"
#include <memory>
#include "../gen/blast_tokens.hpp"
#include "../shader_preprocessed_info.hpp"

std::shared_ptr<gen::BlastScanner> buildScanner();

void ParserCommon::parse(const ShaderPreprocessedInfo &preprocessedInfo)
{
	auto scanner = buildScanner();
	auto errorCallback = std::function([](const gen::location &loc, const std::string &msg) -> void
	{

	});
	Context ctx;
	gen::BlastParser parser(scanner.get(), errorCallback, ctx);
	parser.parse();
}
