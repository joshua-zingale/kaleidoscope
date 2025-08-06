cmake_minimum_required(VERSION 3.10.0)
project(KaleidoscopeTutorial VERSION 0.1.0 LANGUAGES CXX)
add_subdirectory(kaleidoscope)

option(BUILD_TESTS "Build lexer test executable" OFF)
