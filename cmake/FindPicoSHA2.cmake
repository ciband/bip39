#FindPicoSHA2.cmake
# 
# Finds the PicoSHA2 library
#
# This will define the following variables
#
#    PicoSHA2_INCLUDE_DIRS
#
# and the following imported targets
#
#     PicoSHA2::PicoSHA2
#
# Author: Pablo Arias - pabloariasal@gmail.com
# Adapted by ciband

find_path(PicoSHA2_INCLUDE_DIR
    NAMES picosha2.h
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib/PicoSHA2
    PATH_SUFFIXES picosha2
)

if((NOT PicoSHA2_INCLUDE_DIR) OR (NOT EXISTS ${PicoSHA2_INCLUDE_DIR}))
    # we couldn't find the header files for PicoSHA2 or they don't exist
    message("Unable to find PicoSHA2. Cloning from git...")

    # we have a submodule setup for PicoSHA2, 
    # now we need to clone this submodule
    execute_process(COMMAND git submodule update --init
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    # set PicoSHA2_INCLUDE_DIR properly
    set(PicoSHA2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/PicoSHA2)
    
    
else()
    # see above, setup target as well
endif()

mark_as_advanced(PicoSHA2_FOUND PicoSHA2_INCLUDE_DIR PicoSHA2_VERSION)
