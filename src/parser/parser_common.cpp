#include "parser_common.hpp"
#include <memory>
#include <iostream>
#include "../gen/blast_tokens.hpp"
#include "../shader_preprocessed_info.hpp"

std::shared_ptr<gen::BlastScanner> buildScanner(const std::string &source);
std::shared_ptr<Context> buildContext();
std::shared_ptr<AbstractSyntaxTreeContainer> getContainer(std::shared_ptr<Context> context);

std::shared_ptr<AbstractSyntaxTreeContainer> ParserCommon::parse(const ShaderPreprocessedInfo &preprocessedInfo)
{
	auto scanner = buildScanner(preprocessedInfo.text());
	auto sourceFiles = preprocessedInfo.sourceFiles();
	auto debugRowsInfo = preprocessedInfo.debugRowsInfo();
	auto errorCallback = std::function([&sourceFiles, &debugRowsInfo](const gen::location &loc, const std::string &msg) -> void
	{
		auto begin = debugRowsInfo[loc.begin.line];
		auto end = debugRowsInfo[loc.end.line];
		auto sourceFileBegin = sourceFiles[begin.sourceFileId];
		auto sourceFileEnd = sourceFiles[end.sourceFileId];
		std::cerr << msg << " started  in file " << sourceFileBegin << " on " << begin.sourceRowNumber << ":" << loc.begin.column
			<< ", ended in file " << sourceFileEnd << " on " << end.sourceRowNumber << ":" << loc.end.column << std::endl;
	});
	auto context = buildContext();
	gen::BlastParser parser(scanner.get(), errorCallback, *context);
	parser.parse();
	return getContainer(context);
}
