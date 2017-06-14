cmake_minimum_required(VERSION 3.7)
project(testcpp)

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
        Test.C)

add_executable(testcpp ${SOURCE_FILES})