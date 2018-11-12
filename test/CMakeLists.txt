cmake_minimum_required(VERSION 3.2)
project(bip39_tests C CXX)

add_subdirectory(lib/googletest)

include_directories(${BIP39_HEADERS_DIR})
include_directories(lib/googletest/googletest/include)

set(SOURCE_FILES main.cpp src/bip39_tests.cpp)

add_executable(bip39_tests ${SOURCE_FILES})
target_link_libraries(bip39_tests bip39 gtest)
install(TARGETS bip39_tests DESTINATION bin)
