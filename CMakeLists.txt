cmake_minimum_required(VERSION 4.2)
project(RAM_Machine_ASM_Compiler_Interpreter_V2 C)

set(CMAKE_C_STANDARD 11)

# Exécutable principal
add_executable(ramc
        src/main.c
        src/myarray.c
        src/mystring.c
        src/machine.c
        src/interpreter.c
        src/compiler.c
)
target_include_directories(ramc PRIVATE include/)

# Tests
enable_testing()

add_executable(test_blank tests/test_blank.c)
target_include_directories(test_blank PRIVATE include/)
add_test(NAME test_blank COMMAND test_blank)

add_executable(test_machine src/machine.c tests/test_machine.c src/myarray.c)
target_include_directories(test_machine PRIVATE include/)
add_test(NAME test_machine COMMAND test_machine)

add_executable(test_interpreter src/interpreter.c src/machine.c tests/test_interpreter.c src/myarray.c)
target_include_directories(test_interpreter PRIVATE include/)
add_test(NAME test_interpreter COMMAND test_interpreter)

add_executable(test_compiler src/compiler.c src/interpreter.c src/machine.c tests/test_compiler.c src/myarray.c src/mystring.c)
target_include_directories(test_compiler PRIVATE include/)
add_test(NAME test_compiler COMMAND test_compiler)

# Variables
add_compile_definitions(PROJECT_ROOT="${CMAKE_SOURCE_DIR}")