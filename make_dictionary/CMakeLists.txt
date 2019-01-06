cmake_minimum_required(VERSION 3.2)
project(bip39_make_dictionary C CXX)

find_library(bip39 PUBLIC)

set(SOURCE_FILES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} bip39)

if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
  target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()
