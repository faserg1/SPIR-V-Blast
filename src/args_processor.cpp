#include "args_parser.hpp"
#include "args_processor.hpp"

#include <algorithm>

#include "preprocessor.hpp"
#include "reader.hpp"
#include "translator/translator.hpp"

void processPreprocessorFlags(Preprocessor &preprocessor, const std::vector<Option> &options)
{
	std::vector<Option> defineOptions;
	std::copy_if(options.begin(), options.end(), std::back_inserter(defineOptions), [](const Option &opt) -> bool
	{
		return opt.name == "-D" || opt.name == "--define";
	});

	for (auto &opt : defineOptions)
	{
		auto &arg = opt.optionArgs[0];
		auto eqCount = std::count(arg.begin(), arg.end(), '=');
		if (eqCount > 1)
			throw std::runtime_error("Wrong define! " + arg);
		if (!eqCount)
			preprocessor.addDefine(arg);
		else
		{
			auto iter = std::find(arg.begin(), arg.end(), '=');
			auto idx = static_cast<size_t>(std::distance(arg.begin(), iter));
			std::string key = arg.substr(0, idx);
			std::string value = arg.substr(idx + 1);
			preprocessor.addDefine(key, value);
		}
	}
}

void processReaderFlags(Reader &reader, const std::vector<Option> &options)
{
	std::vector<Option> includeOptions;
	std::copy_if(options.begin(), options.end(), std::back_inserter(includeOptions), [](const Option &opt) -> bool
	{
		return opt.name == "-I" || opt.name == "--include";
	});

	for (auto &opt : includeOptions)
	{
		auto &arg = opt.optionArgs[0];
		auto eqCount = std::count(arg.begin(), arg.end(), '=');
		if (eqCount != 1)
			throw std::runtime_error("Wrong include directory! " + arg);
		else
			reader.addSearchPath(arg);
	}
}

void processTranslatorFlags(Translator &translator, const std::vector<Option> &options)
{
	for (auto &opt : options)
	{
		if (opt.name == "-h" || opt.name == "--human-readable")
			translator.setHumanReadable();
		if (opt.name == "-b" || opt.name == "--binary")
			translator.setHumanReadable(false);
	}
}