cmake_minimum_required(VERSION 3.0.0)
project(Fractions VERSION 0.1.0)

include(CTest)
enable_testing()

add_compile_options(-std=c++20)

add_executable(Fractions main.cpp driver.cpp parser.cpp scanner.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_VERBOSE_MAKEFILE ON)

include(CPack)
