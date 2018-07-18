cmake_minimum_required(VERSION 3.10)
include_directories(. ${CMAKE_BINARY_DIR})


set(CMAKE_CXX_STANDARD 11)
find_package(BISON)
find_package(FLEX)
find_package(LLVM)
find_program(LLVM_CONFIG NAMES llvm-config)
#set(CMAKE_CXX_FLAGS )
#SET(PARSER_DIR ${CMAKE_SOURCE_DIR}/parser)

if (LLVM_CONFIG)
    message("-- find llvm-config")
    execute_process(COMMAND ${LLVM_CONFIG} --cxxflags OUTPUT_VARIABLE LLVM_CXXFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (LLVM_CXXFLAGS)
        message("-- find llvm-config --cxxflags")
    else ()
        message("-- could not find llvm-config --cxxflags" )
    endif()
    execute_process(COMMAND ${LLVM_CONFIG} --ldflags OUTPUT_VARIABLE LLVM_LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${LLVM_CONFIG} --libs OUTPUT_VARIABLE LLVM_LIBS)
else()
    message(WARNING "Could not find llvm-config")
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_CXXFLAGS}")
message("cmake_cxx_flags ${CMAKE_CXX_FLAGS}")
message("${LIBCLANG_CXXFLAGS}")
message("${LLVM_LIBS}")

BISON_TARGET(ToyParser parser/toylang.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)

FLEX_TARGET(ToyFlex parser/toylang.l
        ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)
include_directories(. ${CMAKE_CURRENT_BINARY_DIR})


SET(COMPILER_SOURCE compile/node.cc compile/node.h compile/codegen.h compile/codegen.cc)
add_executable(toy main.cc ${BISON_ToyParser_OUTPUTS} ${FLEX_ToyFlex_OUTPUTS} ${COMPILER_SOURCE} )
target_link_libraries(toy ${LLVM_LIBS})
#target_link_libraries()
#add_executable(xlang main.cc compile/node.h)
