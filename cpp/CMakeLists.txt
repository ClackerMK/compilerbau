cmake_minimum_required(VERSION 3.7)
project(testcpp)

find_package(LLVM REQUIRED CONFIG)


message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_STANDARD 11)


set(SOURCE_FILES
        Absyn.C
        Absyn.H
        Lexer.C
        Parser.C
        Parser.H
        Printer.C
        Printer.H
        TypeChecker.C
        TypeChecker.H
        SymbolTableBuilder.C
        SymbolTableBuilder.H
        ConstantFolder.C
        ConstantFolder.H
        CodeGenerator.C
        CodeGenerator.H
        Test.C)


include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(testcpp ${SOURCE_FILES})

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(testcpp ${llvm_libs})
