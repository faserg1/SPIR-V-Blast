cmake_minimum_required (VERSION 3.12)
project (SPIRV-V-Blast C CXX)

set (CMAKE_CXX_STANDARD 17)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#prepare config
include (Arch.cmake)
include (include.cmake)
include (thirdparty.cmake)

#dirs
set(BLAST_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(BLAST_LANG_DIR ${BLAST_ROOT}/lang)
set(BLAST_GEN_DIR ${BLAST_ROOT}/src/gen)

set(BLAST_ROOT_DIR ${BLAST_ROOT}/built/${TARGET_ARCH})
set(BIN_BUILD_OUTPUT_DIRECTORY ${BLAST_ROOT_DIR}/bin)
set(BUILD_OUTPUT_DIRECTORY ${BIN_BUILD_OUTPUT_DIRECTORY})

set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY})
set(LIBRARY_OUTPUT_PATH  ${BUILD_OUTPUT_DIRECTORY})
set(RUNTIME_OUTPUT_DIRECTORY  ${BUILD_OUTPUT_DIRECTORY})

#sources
add_subdirectory (src)

#special
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Blast)


#enum generator
add_custom_target(
	enum_generator ALL
	DEPENDS ${BLAST_GEN_DIR}/spirv_enums.hpp
)

add_custom_command(
	OUTPUT
		${BLAST_GEN_DIR}/spirv_enums.hpp
		${BLAST_GEN_DIR}/spirv_enums.cpp
	WORKING_DIRECTORY ${BLAST_ROOT}
	COMMAND ${PYTHON_EXECUTABLE} generateConverter.py)

add_dependencies(Blast enum_generator)
