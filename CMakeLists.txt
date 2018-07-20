cmake_minimum_required(VERSION 3.10)
include_directories(. ${CMAKE_BINARY_DIR})


set(CMAKE_CXX_STANDARD 11)
find_package(BISON)
find_package(FLEX)
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
#llvm_map_components_to_libnames(llvm_libs support core irreader AsmParser
#        LTO ObjCARCOpts Symbolize DebugInfoPDB DebugInfoDWARF XCoreDisassembler XCoreCodeGen XCoreDesc
#        XCoreInfo XCoreAsmPrinter SystemZDisassembler SystemZCodeGen SystemZAsmParser SystemZDesc
#        SystemZInfo SystemZAsmPrinter SparcDisassembler SparcCodeGen SparcAsmParser SparcDesc SparcInfo
#        SparcAsmPrinter PowerPCDisassembler PowerPCCodeGen PowerPCAsmParser PowerPCDesc PowerPCInfo
#        PowerPCAsmPrinter NVPTXCodeGen NVPTXDesc NVPTXInfo NVPTXAsmPrinter MSP430CodeGen MSP430Desc MSP430Info
#        MSP430AsmPrinter MipsDisassembler MipsCodeGen MipsAsmParser MipsDesc MipsInfo MipsAsmPrinter
#        HexagonDisassembler HexagonCodeGen HexagonAsmParser HexagonDesc HexagonInfo CppBackendCodeGen CppBackendInfo
#        BPFCodeGen BPFDesc BPFInfo BPFAsmPrinter ARMDisassembler ARMCodeGen ARMAsmParser ARMDesc ARMInfo ARMAsmPrinter
#        AMDGPUCodeGen AMDGPUAsmParser AMDGPUDesc AMDGPUUtils AMDGPUInfo AMDGPUAsmPrinter AArch64Disassembler AArch64CodeGen
#        AArch64AsmParser AArch64Desc AArch64Info AArch64AsmPrinter AArch64Utils MIRParser LibDriver Option TableGen LineEditor
#        X86Disassembler X86AsmParser X86CodeGen SelectionDAG AsmPrinter X86Desc MCDisassembler X86Info X86AsmPrinter X86Utils
#        MCJIT Passes ipo Vectorize Linker IRReader AsmParser DebugInfoCodeView Interpreter CodeGen ScalarOpts InstCombine
#        Instrumentation ProfileData BitWriter OrcJIT TransformUtils ExecutionEngine Target Analysis RuntimeDyld Object MCParser
#        BitReader MC Core Support
#        )
#llvm_map_components_to_libnames(llvm_libs core support ExecutionEngine )

add_definitions(${LLVM_DEFINITIONS} )
set(CMAKE_CXX_FLAGS  "-rdynamic")
message("${LLVM_DEFINITIONS}" )
#find_program(LLVM_CONFIG NAMES llvm-config)
set(CMAKE_CXX_COMPILER g++-7)
llvm_map_components_to_libnames(llvm_libs
        support core irreader Target CodeGen Passes ExecutionEngine
        X86CodeGen X86Desc X86Info X86AsmParser
        )
message(STATUS "Using LLVM LIBS ${llvm_libs}")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_CXXFLAGS}")
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
include_directories(. ${CMAKE_CURRENT_BINARY_DIR} ${LLVM_INCLUDE_DIRS} )
message(STATUS " ${CMAKE_CURRENT_BINARY_DIR} ")

SET(COMPILER_SOURCE
        compile/node.h
        compile/codegen.h
        compile/codegen.cc
        compile/corefn.cc
        compile/native.cc)
add_executable(toy ${BISON_ToyParser_OUTPUTS} ${FLEX_ToyFlex_OUTPUTS} ${COMPILER_SOURCE}  main.cc )
#target_link_libraries(toy ${llvm_libs} LLVMExecutionEngine pthread dl fl z ncurses )
target_link_libraries(toy  LLVM pthread  dl z ncurses)
#target_link_libraries()
#add_executable(xlang main.cc compile/node.h)
