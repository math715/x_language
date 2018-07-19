cmake_minimum_required(VERSION 3.10)
include_directories(. ${CMAKE_BINARY_DIR})


set(CMAKE_CXX_STANDARD 11)
find_package(BISON)
find_package(FLEX)
#find_package(LLVM)
find_program(LLVM_CONFIG NAMES llvm-config)
set(CMAKE_CXX_COMPILER clang++)
#set(CMAKE_CXX_FLAGS )
#SET(PARSER_DIR ${CMAKE_SOURCE_DIR}/parser)

if (LLVM_CONFIG)
    message("-- find llvm-config ${LLVM_CONFIG}")
    execute_process(COMMAND ${LLVM_CONFIG} --cxxflags OUTPUT_VARIABLE LLVM_CXXFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (LLVM_CXXFLAGS)
        message("-- find llvm-config --cxxflags ${LLVM_CXXFLAGS}")
    else ()
        message("-- could not find llvm-config --cxxflags" )
    endif()
    execute_process(COMMAND ${LLVM_CONFIG} --ldflags OUTPUT_VARIABLE LLVM_LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (LLVM_LDFLAGS)
        message("-- find llvm-config --ldflafs ${LLVM_LDFLAGS}")
    endif()
    execute_process(COMMAND ${LLVM_CONFIG} --libs OUTPUT_VARIABLE LLVM_LIBS)
    if (LLVM_LIBS)
        message("-- find llvm-config --libs ${LLVM_LIBS}")
    endif()
else()
    message(WARNING "Could not find llvm-config")
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_CXXFLAGS}")
#message("cmake_cxx_flags ${CMAKE_CXX_FLAGS}")
#message("${LLVM_LDFLAGS}")
#message("${LLVM_LIBS}")

BISON_TARGET(ToyParser parser/toylang.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)

FLEX_TARGET(ToyFlex parser/toylang.l
        ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)
message(${BISON_ToyParser_OUTPUTS})
message(${FLEX_ToyFlex_OUTPUTS})
include_directories(. ${CMAKE_CURRENT_BINARY_DIR})

SET(COMPILER_SOURCE
        compile/node.h
        compile/codegen.h
        compile/codegen.cc
        )
add_executable(toy main.cc ${BISON_ToyParser_OUTPUTS} ${FLEX_ToyFlex_OUTPUTS} ${COMPILER_SOURCE} )
target_link_libraries(toy LLVM pthread dl z ncurses -rdynamic )
#target_link_libraries()
#add_executable(xlang main.cc compile/node.h)
