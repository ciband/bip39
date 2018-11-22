cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

if (MSVC)
	add_definitions(
		-D_CRT_SECURE_NO_WARNINGS
		-D_SCL_SECURE_NO_WARNINGS
		-DNOMINMAX
	)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

# clone submodules
execute_process(
	COMMAND git submodule update --init --recursive
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)

