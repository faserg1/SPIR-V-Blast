#ifndef ARGS_PROCESSOR
#define ARGS_PROCESSOR

#include <vector>

struct Option;
class Preprocessor;
class Reader;
class Translator;
class Writer;

void processPreprocessorFlags(Preprocessor &preprocessor, const std::vector<Option> &options);
void processReaderFlags(Reader &reader, const std::vector<Option> &options);
void processTranslatorFlags(Translator &translator, const std::vector<Option> &options);
void processWriterFlags(Writer &writer, const std::vector<Option> &options);

#endif // ARGS_PROCESSOR