cmake_minimum_required(VERSION 3.2)
project(bip39_tests C CXX)

set(BUILD_GMOCK OFF FORCE)

add_subdirectory(lib/googletest)

find_library(bip39 PUBLIC)

set(SOURCE_FILES
    src/IoT/main.cpp
    src/bip39_tests.cpp
    src/word_list_tests.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} bip39 gtest gtest_main)

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    include("${CMAKE_SOURCE_DIR}/cmake/CodeCoverage.cmake")

    setup_target_for_coverage(${PROJECT_NAME}_coverage bip39_tests coverage)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
endif() #CMAKE_BUILD_TYPE STREQUAL "Coverage"
