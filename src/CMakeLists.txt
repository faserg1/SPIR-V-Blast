cmake_minimum_required (VERSION 3.12)

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}) 

set(BlastHeaders
	${SOURCE_DIR}/args.hpp
	${SOURCE_DIR}/args_processor.hpp
	${SOURCE_DIR}/reader.hpp
	${SOURCE_DIR}/shader.hpp
	${SOURCE_DIR}/compiler_types.hpp
	${SOURCE_DIR}/compiler_nodes.hpp
	${SOURCE_DIR}/compiler_states.hpp
	${SOURCE_DIR}/compiler_types_parser.hpp
	${SOURCE_DIR}/compiler_parser.hpp
	${SOURCE_DIR}/compiler_parsers.hpp
	${SOURCE_DIR}/compiler_state_machine.hpp
	${SOURCE_DIR}/compiler.hpp
	${SOURCE_DIR}/preprocessor.hpp
	${SOURCE_DIR}/shader_preprocessed_info.hpp
)

set(BlastSources
	${SOURCE_DIR}/args.cpp
	${SOURCE_DIR}/args_processor.cpp
	${SOURCE_DIR}/reader.cpp
	${SOURCE_DIR}/shader.cpp
	${SOURCE_DIR}/compiler_types.cpp
	${SOURCE_DIR}/compiler_nodes.cpp
	${SOURCE_DIR}/compiler_states.cpp
	${SOURCE_DIR}/compiler_types_parser.cpp
	${SOURCE_DIR}/compiler_parser.cpp
	${SOURCE_DIR}/compiler_state_machine.cpp
	${SOURCE_DIR}/compiler.cpp
	${SOURCE_DIR}/preprocessor.cpp
	${SOURCE_DIR}/shader_preprocessed_info.cpp
	${SOURCE_DIR}/main.cpp
)

add_executable(Blast ${BlastHeaders} ${BlastSources})