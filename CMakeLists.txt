cmake_minimum_required(VERSION 3.10)

project(simple-vsat)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/minisat/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()


add_subdirectory(minisat)

set(SOURCES
    minisat/minisat/utils/Options.cc
    minisat/minisat/utils/System.cc
    minisat/minisat/core/Solver.cc
    minisat/minisat/simp/SimpSolver.cc
    lib/minisat.cpp)

# Add libs
add_library(simple-vsat-lib ${SOURCES})

add_executable(simple-vsat Main.cpp)

# Link simple-vsat libs
target_link_libraries(simple-vsat simple-vsat-lib)

# Link minisat libs
#target_link_libraries(simple-vsat minisat-lib-static )