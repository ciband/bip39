cmake_minimum_required(VERSION 3.2)
project(bip39 C CXX)

include_directories(lib/PicoSHA2)

set(SOURCE_FILES
    bip39.h
    bip39.cpp
	en.h
	en_str.h
	util.h
	util.cpp
)

add_library(bip39 SHARED STATIC ${SOURCE_FILES})
