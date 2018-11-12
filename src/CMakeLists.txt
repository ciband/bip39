cmake_minimum_required(VERSION 3.2)
project(bip39 C CXX)

set(SOURCE_FILES
    bip39.h
    bip39.cpp
	en.h
	en_str.h
	util.h
	util.cpp
)

add_library(bip39 STATIC ${SOURCE_FILES})

install(TARGETS bip39 DESTINATION ${BIP39_INSTALL_LIB_DIR})
install(FILES bip39.h en.h en_str.h util.h  DESTINATION ${BIP39_INSTALL_INCLUDE_DIR})