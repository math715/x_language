cmake_minimum_required(VERSION 3.10)
include_directories(. ${CMAKE_BINARY_DIR})

find_package(BISON)
find_package(FLEX)


#SET(PARSER_DIR ${CMAKE_SOURCE_DIR}/parser)



BISON_TARGET(ToyParser parser/toylang.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)

FLEX_TARGET(ToyFlex parser/toylang.l
        ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)
include_directories(. ${CMAKE_CURRENT_BINARY_DIR})

SET(COMPILER_SOURCE compile/node.cc compile/node.h compile/codegen.h compile/codegen.cc)
add_executable(toy main.cc ${BISON_ToyParser_OUTPUTS} ${FLEX_ToyFlex_OUTPUTS} ${COMPILER_SOURCE} )
#add_executable(xlang main.cc compile/node.h)
