cmake_minimum_required(VERSION 3.2)
project(bip39_example C CXX)

find_library(bip39 PUBLIC)

set(SOURCE_FILES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} bip39)
