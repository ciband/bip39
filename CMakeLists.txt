cmake_minimum_required(VERSION 3.5)
project(bip39 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_INSTALL_LIBDIR ${PROJECT_SOURCE_DIR}/lib)

#Make sure that custom modules like FindRapidJSON are found
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)


if (MSVC)
	add_definitions(
		-D_CRT_SECURE_NO_WARNINGS
		-D_SCL_SECURE_NO_WARNINGS
		-DNOMINMAX
	)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

##############################################
# Declare dependencies
find_package(PicoSHA2 REQUIRED MODULE)

##############################################
# Create target and set properties
set(SOURCE_FILES
    src/bip39.cpp
	src/util.cpp
)

add_library(bip39 SHARED STATIC ${SOURCE_FILES})

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(bip39::bip39 ALIAS bip39)

#Set target properties
target_include_directories(bip39
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${PicoSHA2_INCLUDE_DIR}
)

target_compile_features(bip39 PRIVATE cxx_auto_type)
target_compile_options(bip39 PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

#target_link_libraries(jsonutils
#    PUBLIC
#        Boost::boost RapidJSON::RapidJSON
#    PRIVATE
#        Boost::regex
#)

##############################################
# Installation instructions

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/bip39)

install(TARGETS bip39
    EXPORT bip39-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#This is required so that the exported target has the name JSONUtils and not jsonutils
#set_target_properties(bip39 PROPERTIES EXPORT_NAME bip39)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#Export the targets to a script
install(EXPORT bip39-targets
  FILE
    bip39Targets.cmake
  NAMESPACE
    bip39::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/bip39ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/bip39Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/bip39Config.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_LIST_DIR}/cmake/FindPicoSHA2.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/bip39Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/bip39ConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

##############################################
## Exporting from the build tree
configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/FindPicoSHA2.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/FindPicoSHA2.cmake
    COPYONLY)

export(EXPORT bip39-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/bip39Targets.cmake NAMESPACE bip39::)

#Register package in user's package registry
export(PACKAGE bip39)

add_subdirectory(test)
