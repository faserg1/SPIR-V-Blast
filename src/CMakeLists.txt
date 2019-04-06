cmake_minimum_required (VERSION 3.12)

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}) 
set(SOURCE_PARSER_DIR ${SOURCE_DIR}/parser)
set(SOURCE_COMPILER_DIR ${SOURCE_DIR}/compiler)
set(SOURCE_TRANSLATOR_DIR ${SOURCE_DIR}/translator)
set(SOURCE_BLAST_DIR ${SOURCE_DIR}/blast)

set(BlastParserHeaders
	${SOURCE_PARSER_DIR}/parser_node_type_enum.hpp
	${SOURCE_PARSER_DIR}/parser_nodes.hpp
	${SOURCE_PARSER_DIR}/parser_types.hpp
	${SOURCE_PARSER_DIR}/parser_state_enum.hpp
	${SOURCE_PARSER_DIR}/parser_state.hpp
	${SOURCE_PARSER_DIR}/parser_states.hpp
	${SOURCE_PARSER_DIR}/parser_states_generator.hpp
	${SOURCE_PARSER_DIR}/parser_symbols.hpp
	${SOURCE_PARSER_DIR}/parser_symbols_parser.hpp
	${SOURCE_PARSER_DIR}/parser_types_parser.hpp
	${SOURCE_PARSER_DIR}/common_parser.hpp
	${SOURCE_PARSER_DIR}/parsers.hpp
	${SOURCE_PARSER_DIR}/parser_state_machine.hpp
	${SOURCE_PARSER_DIR}/parser_expressions.hpp
	${SOURCE_PARSER_DIR}/parser_expressions_parser.hpp
	${SOURCE_PARSER_DIR}/parser_expression_level_enum.hpp
	${SOURCE_PARSER_DIR}/parser_expression_stack.hpp
	${SOURCE_PARSER_DIR}/parser_expression_border_state.hpp
	${SOURCE_PARSER_DIR}/parser.hpp
)

set(BlastParserSources
	${SOURCE_PARSER_DIR}/parser_nodes.cpp
	${SOURCE_PARSER_DIR}/parser_types.cpp
	${SOURCE_PARSER_DIR}/parser_state.cpp
	${SOURCE_PARSER_DIR}/parser_states.cpp
	${SOURCE_PARSER_DIR}/parser_states_generator.cpp
	${SOURCE_PARSER_DIR}/parser_symbols.cpp
	${SOURCE_PARSER_DIR}/parser_symbols_parser.cpp
	${SOURCE_PARSER_DIR}/parser_types_parser.cpp
	${SOURCE_PARSER_DIR}/common_parser.cpp
	${SOURCE_PARSER_DIR}/parser_state_machine.cpp
	${SOURCE_PARSER_DIR}/parser_expressions.cpp
	${SOURCE_PARSER_DIR}/parser_expressions_parser.cpp
	${SOURCE_PARSER_DIR}/parser_expression_stack.cpp
	${SOURCE_PARSER_DIR}/parser_expression_border_state.cpp
	${SOURCE_PARSER_DIR}/parser.cpp
)

set(BlastCompilerHeaders
	${SOURCE_COMPILER_DIR}/compiler.hpp
	${SOURCE_COMPILER_DIR}/compiler_node.hpp
	${SOURCE_COMPILER_DIR}/compiler_node_expression.hpp
	${SOURCE_COMPILER_DIR}/compiler_node_function.hpp
	${SOURCE_COMPILER_DIR}/compiler_node_function_body.hpp
	${SOURCE_COMPILER_DIR}/compiler_node_function_parameter.hpp
	${SOURCE_COMPILER_DIR}/compiler_node_type.hpp
	${SOURCE_COMPILER_DIR}/compiler_node_type_enum.hpp
	${SOURCE_COMPILER_DIR}/compiler_node_variable.hpp
	${SOURCE_COMPILER_DIR}/compiler_state_machine.hpp
	${SOURCE_COMPILER_DIR}/compiler_state.hpp
	${SOURCE_COMPILER_DIR}/compiler_state_enum.hpp
	${SOURCE_COMPILER_DIR}/compiler_state_generator.hpp
)

set(BlastCompilerSources
	${SOURCE_COMPILER_DIR}/compiler.cpp
	${SOURCE_COMPILER_DIR}/compiler_node.cpp
	${SOURCE_COMPILER_DIR}/compiler_node_expression.cpp
	${SOURCE_COMPILER_DIR}/compiler_node_function.cpp
	${SOURCE_COMPILER_DIR}/compiler_node_function_body.cpp
	${SOURCE_COMPILER_DIR}/compiler_node_function_parameter.cpp
	${SOURCE_COMPILER_DIR}/compiler_node_type.cpp
	${SOURCE_COMPILER_DIR}/compiler_node_variable.cpp
	${SOURCE_COMPILER_DIR}/compiler_state_machine.cpp
	${SOURCE_COMPILER_DIR}/compiler_state.cpp
	${SOURCE_COMPILER_DIR}/compiler_state_generator.cpp
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
	${BlastParserSources}
	${BlastCompilerSources}
	${BlastTranslatorSources}
	${BlastBlastSources}
)

source_group("${SOURCE_GROUP_HEADER_START}parser" FILES ${BlastParserHeaders})
source_group("${SOURCE_GROUP_SOURCE_START}parser" FILES ${BlastParserSources})

source_group("${SOURCE_GROUP_HEADER_START}compiler" FILES ${BlastCompilerHeaders})
source_group("${SOURCE_GROUP_SOURCE_START}compiler" FILES ${BlastCompilerSources})

source_group("${SOURCE_GROUP_HEADER_START}translator" FILES ${BlastTranslatorHeaders})
source_group("${SOURCE_GROUP_SOURCE_START}translator" FILES ${BlastTranslatorSources})

source_group("${SOURCE_GROUP_HEADER_START}blast" FILES ${BlastBlastHeaders})
source_group("${SOURCE_GROUP_SOURCE_START}blast" FILES ${BlastBlastSources})

add_executable(Blast ${BlastHeaders} ${BlastSources})