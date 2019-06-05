cmake_minimum_required (VERSION 3.12)

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SOURCE_GENERATED_DIR ${SOURCE_DIR}/gen)
set(SOURCE_PARSER_DIR ${SOURCE_DIR}/parser)
set(SOURCE_COMPILER_DIR ${SOURCE_DIR}/compiler)
set(SOURCE_TRANSLATOR_DIR ${SOURCE_DIR}/translator)

#lang

file(MAKE_DIRECTORY ${SOURCE_GENERATED_DIR})

FLEX_TARGET(BlastTokens
	${BLAST_LANG_DIR}/blast.l
	${BLAST_GEN_DIR}/blast_scanner.hpp
	COMPILE_FLAGS ${FLEX_ARCH_ARGS}
)
BISON_TARGET(BlastParser
	${BLAST_LANG_DIR}/blast.y
	${BLAST_GEN_DIR}/blast_parser.hpp
	DEFINES_FILE ${BLAST_GEN_DIR}/blast_tokens.hpp
)
ADD_FLEX_BISON_DEPENDENCY(BlastTokens BlastParser)

set(BlastParserHeaders
	${SOURCE_PARSER_DIR}/parser_common.hpp
)

set(BlastParserSources
	${SOURCE_PARSER_DIR}/parser_common.cpp
	${SOURCE_PARSER_DIR}/blast_scanner.cpp
	${SOURCE_PARSER_DIR}/blast_parser.cpp
)

set(BlastCompilerHeaders
	${SOURCE_COMPILER_DIR}/compiler_data.hpp
	${SOURCE_COMPILER_DIR}/compiler_common.hpp
	${SOURCE_COMPILER_DIR}/compiler_context.hpp
	${SOURCE_COMPILER_DIR}/compiler_identifiers.hpp
	${SOURCE_COMPILER_DIR}/compiler_validator.hpp
)

set(BlastCompilerSources
	${SOURCE_COMPILER_DIR}/compiler_common.cpp
	${SOURCE_COMPILER_DIR}/compiler_context.cpp
	${SOURCE_COMPILER_DIR}/compiler_identifiers.cpp
	${SOURCE_COMPILER_DIR}/compiler_validator.cpp
)

set(BlastTranslatorHeaders
	${SOURCE_TRANSLATOR_DIR}/translator.hpp
)

set(BlastTranslatorSources
	${SOURCE_TRANSLATOR_DIR}/translator.cpp
)

set(BlastGenerated
	${BISON_BlastParser_OUTPUTS}
	${FLEX_BlastTokens_OUTPUTS}
)

set(BlastHeaders
	${SOURCE_DIR}/args_parser.hpp
	${SOURCE_DIR}/args_processor.hpp
	${SOURCE_DIR}/reader.hpp
	${SOURCE_DIR}/preprocessor.hpp
	${SOURCE_DIR}/shader_preprocessed_info.hpp
	${SOURCE_DIR}/writer.hpp
	${BlastParserHeaders}
	${BlastCompilerHeaders}
	${BlastTranslatorHeaders}
)

set(BlastSources
	${SOURCE_DIR}/args_parser.cpp
	${SOURCE_DIR}/args_processor.cpp
	${SOURCE_DIR}/reader.cpp
	${SOURCE_DIR}/preprocessor.cpp
	${SOURCE_DIR}/shader_preprocessed_info.cpp
	${SOURCE_DIR}/writer.cpp
	${SOURCE_DIR}/main.cpp
	${BlastParserSources}
	${BlastCompilerSources}
	${BlastTranslatorSources}
)

source_group("gen" FILES ${BlastGenerated})

source_group("${SOURCE_GROUP_HEADER_START}parser" FILES ${BlastParserHeaders})
source_group("${SOURCE_GROUP_SOURCE_START}parser" FILES ${BlastParserSources})

source_group("${SOURCE_GROUP_HEADER_START}compiler" FILES ${BlastCompilerHeaders})
source_group("${SOURCE_GROUP_SOURCE_START}compiler" FILES ${BlastCompilerSources})

source_group("${SOURCE_GROUP_HEADER_START}translator" FILES ${BlastTranslatorHeaders})
source_group("${SOURCE_GROUP_SOURCE_START}translator" FILES ${BlastTranslatorSources})

add_executable(Blast ${BlastHeaders} ${BlastSources} ${BlastGenerated})
target_include_directories(Blast PRIVATE ${FLEX_INCLUDE_DIRS})
target_link_libraries(Blast ${ARCH_LIBS})
