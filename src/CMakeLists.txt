cmake_minimum_required (VERSION 3.12)

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}) 
set(SOURCE_COMPILER_DIR ${SOURCE_DIR}/compiler)
set(SOURCE_TRANSLATOR_DIR ${SOURCE_DIR}/compiler)

set(BlastCompilerHeaders
	${SOURCE_COMPILER_DIR}/compiler_types.hpp
	${SOURCE_COMPILER_DIR}/compiler_nodes.hpp
	${SOURCE_COMPILER_DIR}/compiler_states.hpp
	${SOURCE_COMPILER_DIR}/compiler_symbols.hpp
	${SOURCE_COMPILER_DIR}/compiler_symbols_parser.hpp
	${SOURCE_COMPILER_DIR}/compiler_types_parser.hpp
	${SOURCE_COMPILER_DIR}/compiler_parser.hpp
	${SOURCE_COMPILER_DIR}/compiler_parsers.hpp
	${SOURCE_COMPILER_DIR}/compiler_state_machine.hpp
	${SOURCE_COMPILER_DIR}/compiler_expressions.hpp
	${SOURCE_COMPILER_DIR}/compiler_expressions_parser.hpp
	${SOURCE_COMPILER_DIR}/compiler.hpp
)

set(BlastCompilerSources
	${SOURCE_COMPILER_DIR}/compiler_types.cpp
	${SOURCE_COMPILER_DIR}/compiler_nodes.cpp
	${SOURCE_COMPILER_DIR}/compiler_states.cpp
	${SOURCE_COMPILER_DIR}/compiler_symbols.cpp
	${SOURCE_COMPILER_DIR}/compiler_symbols_parser.cpp
	${SOURCE_COMPILER_DIR}/compiler_types_parser.cpp
	${SOURCE_COMPILER_DIR}/compiler_parser.cpp
	${SOURCE_COMPILER_DIR}/compiler_state_machine.cpp
	${SOURCE_COMPILER_DIR}/compiler_expressions.cpp
	${SOURCE_COMPILER_DIR}/compiler_expressions_parser.cpp
	${SOURCE_COMPILER_DIR}/compiler.cpp
)

set(BlastHeaders
	${SOURCE_DIR}/args.hpp
	${SOURCE_DIR}/args_processor.hpp
	${SOURCE_DIR}/reader.hpp
	${SOURCE_DIR}/shader.hpp
	${SOURCE_DIR}/preprocessor.hpp
	${SOURCE_DIR}/shader_preprocessed_info.hpp
	${BlastCompilerHeaders}
)

set(BlastSources
	${SOURCE_DIR}/args.cpp
	${SOURCE_DIR}/args_processor.cpp
	${SOURCE_DIR}/reader.cpp
	${SOURCE_DIR}/shader.cpp
	${SOURCE_DIR}/preprocessor.cpp
	${SOURCE_DIR}/shader_preprocessed_info.cpp
	${SOURCE_DIR}/main.cpp
	${BlastCompilerSources}
)

source_group("${SOURCE_GROUP_HEADER_START}compiler" FILES ${BlastCompilerHeaders})
source_group("${SOURCE_GROUP_SOURCE_START}compiler" FILES ${BlastCompilerSources})

add_executable(Blast ${BlastHeaders} ${BlastSources})