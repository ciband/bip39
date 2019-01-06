cmake_minimum_required(VERSION 3.2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

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

# ------------------------------------------------------------------------------
# Clang Tidy
# ------------------------------------------------------------------------------

if(ENABLE_CLANG_TIDY)

    find_program(CLANG_TIDY_BIN clang-tidy-7)
    find_program(RUN_CLANG_TIDY_BIN run-clang-tidy-7.py)

    if(CLANG_TIDY_BIN STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
        message(FATAL_ERROR "unable to locate clang-tidy-7")
    endif()

   if(RUN_CLANG_TIDY_BIN STREQUAL "RUN_CLANG_TIDY_BIN-NOTFOUND")
       message(FATAL_ERROR "unable to locate run-clang-tidy-7.py")
   endif()

    list(APPEND RUN_CLANG_TIDY_BIN_ARGS
        -clang-tidy-binary ${CLANG_TIDY_BIN}
        "\"-header-filter=.*\\b(src|test|examples)\\b\\/(?!lib).*\""     #Only run clang tidy on src, test, examples and skip 3rd party libraries
        -checks=clan*,cert*,misc*,perf*,cppc*,read*,mode*,-cert-err58-cpp,-misc-noexcept-move-constructor,-cppcoreguidelines-*
    )

    add_custom_target(
        tidy
        COMMAND ${RUN_CLANG_TIDY_BIN} ${RUN_CLANG_TIDY_BIN_ARGS}
        COMMENT "running clang tidy"
    )

endif()

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)
add_subdirectory(make_dictionary)

