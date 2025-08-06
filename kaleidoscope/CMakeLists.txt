cmake_minimum_required(VERSION 3.10.0)
add_executable(kaleidoscope main.cpp)

if(BUILD_TESTS)
    add_executable(test_lexer lexer.test.cpp lexer.cpp)
endif()