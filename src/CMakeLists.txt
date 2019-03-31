cmake_minimum_required (VERSION 3.12)

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}) 
set(SOURCE_PARSER_DIR ${SOURCE_DIR}/parser)
set(SOURCE_COMPILER_DIR ${SOURCE_DIR}/compiler)
set(SOURCE_TRANSLATOR_DIR ${SOURCE_DIR}/translator)

set(BlastParserHeaders
	${SOURCE_PARSER_DIR}/parser_types.hpp
	${SOURCE_PARSER_DIR}/parser_state_enum.hpp
	${SOURCE_PARSER_DIR}/parser_states.hpp
	${SOURCE_PARSER_DIR}/parser_symbols.hpp
	${SOURCE_PARSER_DIR}/parser_symbols_parser.hpp
	${SOURCE_PARSER_DIR}/parser_types_parser.hpp
	${SOURCE_PARSER_DIR}/common_parser.hpp
	${SOURCE_PARSER_DIR}/parsers.hpp
	${SOURCE_PARSER_DIR}/parser_state_machine.hpp
	${SOURCE_PARSER_DIR}/parser_expressions.hpp
	${SOURCE_PARSER_DIR}/parser_expressions_parser.hpp
	${SOURCE_PARSER_DIR}/parser.hpp
)

set(BlastParserSources
	${SOURCE_PARSER_DIR}/parser_types.cpp
	${SOURCE_PARSER_DIR}/parser_states.cpp
	${SOURCE_PARSER_DIR}/parser_symbols.cpp
	${SOURCE_PARSER_DIR}/parser_symbols_parser.cpp
	${SOURCE_PARSER_DIR}/parser_types_parser.cpp
	${SOURCE_PARSER_DIR}/common_parser.cpp
	${SOURCE_PARSER_DIR}/parser_state_machine.cpp
	${SOURCE_PARSER_DIR}/parser_expressions.cpp
	${SOURCE_PARSER_DIR}/parser_expressions_parser.cpp
	${SOURCE_PARSER_DIR}/parser.cpp
)

set(BlastCompilerHeaders
	${SOURCE_COMPILER_DIR}/compiler_node_type_enum.hpp
	${SOURCE_COMPILER_DIR}/compiler_nodes.hpp
)

set(BlastCompilerSources
	${SOURCE_COMPILER_DIR}/compiler_nodes.cpp
)

set(BlastHeaders
	${SOURCE_DIR}/args_parser.hpp
	${SOURCE_DIR}/args_processor.hpp
	${SOURCE_DIR}/reader.hpp
	${SOURCE_DIR}/shader.hpp
	${SOURCE_DIR}/preprocessor.hpp
	${SOURCE_DIR}/shader_preprocessed_info.hpp
	${BlastParserHeaders}
	${BlastCompilerHeaders}
)

set(BlastSources
	${SOURCE_DIR}/args_parser.cpp
	${SOURCE_DIR}/args_processor.cpp
	${SOURCE_DIR}/reader.cpp
	${SOURCE_DIR}/shader.cpp
	${SOURCE_DIR}/preprocessor.cpp
	${SOURCE_DIR}/shader_preprocessed_info.cpp
	${SOURCE_DIR}/main.cpp
	${BlastParserSources}
	${BlastCompilerSources}
)

source_group("${SOURCE_GROUP_HEADER_START}parser" FILES ${BlastParserHeaders})
source_group("${SOURCE_GROUP_SOURCE_START}parser" FILES ${BlastParserSources})

source_group("${SOURCE_GROUP_HEADER_START}compiler" FILES ${BlastCompilerHeaders})
source_group("${SOURCE_GROUP_SOURCE_START}compiler" FILES ${BlastCompilerSources})

add_executable(Blast ${BlastHeaders} ${BlastSources})