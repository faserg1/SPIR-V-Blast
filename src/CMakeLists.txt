cmake_minimum_required (VERSION 3.12)

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}) 
set(SOURCE_GENERATED_DIR ${SOURCE_DIR}/gen)
set(SOURCE_PARSER_DIR ${SOURCE_DIR}/parser)
set(SOURCE_COMPILER_DIR ${SOURCE_DIR}/compiler)
set(SOURCE_TRANSLATOR_DIR ${SOURCE_DIR}/translator)
set(SOURCE_BLAST_DIR ${SOURCE_DIR}/blast)

set(BlastParserHeaders

)

set(BlastParserSources

)

set(BlastCompilerHeaders

)

set(BlastCompilerSources

)

set(BlastTranslatorHeaders
	${SOURCE_TRANSLATOR_DIR}/translator.hpp
)

set(BlastTranslatorSources
	${SOURCE_TRANSLATOR_DIR}/translator.cpp
)

set(BlastBlastHeaders
	${SOURCE_BLAST_DIR}/blast_types.hpp
)

set(BlastBlastSources
)

set(BlastGenerated
	#${BISON_BlastParser_OUTPUTS}
)

set(BlastHeaders
	${SOURCE_DIR}/args_parser.hpp
	${SOURCE_DIR}/args_processor.hpp
	${SOURCE_DIR}/reader.hpp
	${SOURCE_DIR}/shader.hpp
	${SOURCE_DIR}/preprocessor.hpp
	${SOURCE_DIR}/shader_preprocessed_info.hpp
	${SOURCE_DIR}/writer.hpp
	${BlastParserHeaders}
	${BlastCompilerHeaders}
	${BlastTranslatorHeaders}
	${BlastBlastHeaders}
)

set(BlastSources
	${SOURCE_DIR}/args_parser.cpp
	${SOURCE_DIR}/args_processor.cpp
	${SOURCE_DIR}/reader.cpp
	${SOURCE_DIR}/shader.cpp
	${SOURCE_DIR}/preprocessor.cpp
	${SOURCE_DIR}/shader_preprocessed_info.cpp
	${SOURCE_DIR}/writer.cpp
	${SOURCE_DIR}/main.cpp
	${BlastGenerated}
	${BlastParserSources}
	${BlastCompilerSources}
	${BlastTranslatorSources}
	${BlastBlastSources}
)

source_group("gen" FILES ${BlastGenerated})

source_group("${SOURCE_GROUP_HEADER_START}parser" FILES ${BlastParserHeaders})
source_group("${SOURCE_GROUP_SOURCE_START}parser" FILES ${BlastParserSources})

source_group("${SOURCE_GROUP_HEADER_START}compiler" FILES ${BlastCompilerHeaders})
source_group("${SOURCE_GROUP_SOURCE_START}compiler" FILES ${BlastCompilerSources})

source_group("${SOURCE_GROUP_HEADER_START}translator" FILES ${BlastTranslatorHeaders})
source_group("${SOURCE_GROUP_SOURCE_START}translator" FILES ${BlastTranslatorSources})

source_group("${SOURCE_GROUP_HEADER_START}blast" FILES ${BlastBlastHeaders})
source_group("${SOURCE_GROUP_SOURCE_START}blast" FILES ${BlastBlastSources})

add_executable(Blast ${BlastHeaders} ${BlastSources})

#lang
ADD_CUSTOM_COMMAND(
	TARGET Blast
	PRE_BUILD 
	COMMAND ${BISON_EXECUTABLE} --report=all -d --output=${BLAST_GEN_DIR}/blast_parser.hpp ${BLAST_LANG_DIR}/blast.y
	COMMENT "Generating blast_parser.hpp"
)