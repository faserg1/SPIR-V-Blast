cmake_minimum_required (VERSION 3.12)

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}) 

set(BlastHeaders
	${SOURCE_DIR}/args.hpp
	${SOURCE_DIR}/reader.hpp
	${SOURCE_DIR}/shader.hpp
	${SOURCE_DIR}/compiler.hpp
	${SOURCE_DIR}/preprocessor.hpp
	${SOURCE_DIR}/shader_preprocessed_info.hpp
)

set(BlastSources
	${SOURCE_DIR}/args.cpp
	${SOURCE_DIR}/reader.cpp
	${SOURCE_DIR}/shader.cpp
	${SOURCE_DIR}/compiler.cpp
	${SOURCE_DIR}/preprocessor.cpp
	${SOURCE_DIR}/shader_preprocessed_info.cpp
	${SOURCE_DIR}/main.cpp
)

add_executable(Blast ${BlastHeaders} ${BlastSources})