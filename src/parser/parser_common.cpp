#include "parser_common.hpp"
#include <memory>
#include "../gen/blast_tokens.hpp"
#include "../shader_preprocessed_info.hpp"

std::shared_ptr<gen::BlastScanner> buildScanner(const std::string &source);
std::shared_ptr<Context> buildContext();
std::shared_ptr<AbstractSyntaxTreeContainer> getContainer(std::shared_ptr<Context> context);

void ParserCommon::parse(const ShaderPreprocessedInfo &preprocessedInfo)
{
	auto scanner = buildScanner(preprocessedInfo.text());
	auto errorCallback = std::function([](const gen::location &loc, const std::string &msg) -> void
	{

	});
	auto context = buildContext();
	gen::BlastParser parser(scanner.get(), errorCallback, *context);
	parser.parse();
	auto ast = getContainer(context);
}
