cmake_minimum_required(VERSION 3.0.0)
project(algeblah VERSION 0.1.0)

set(arbit_prec ON)
set(debug ON)
set(plot ON)

include(CTest)
enable_testing()

if(arbit_prec)
    add_compile_definitions(ARBIT_PREC)
endif()

if (plot)
add_compile_definitions(GNUPLOT)
endif()

if(debug)
    add_compile_definitions(DEBUG)
    add_compile_options(-ggdb)
else()
    add_compile_definitions(NDEBUG -O2)
endif()

add_compile_options(-std=c++17 -Wall -Werror)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(parser parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h COMPILE_FLAGS -Werror)
FLEX_TARGET(scanner scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(algeblah options.cpp main.cpp driver.cpp ${BISON_parser_OUTPUTS} ${FLEX_scanner_OUTPUTS})

if(arbit_prec)
    target_link_libraries(algeblah readline mpfr)
else()
    target_link_libraries(algeblah readline)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_VERBOSE_MAKEFILE ON)

include(CPack)
