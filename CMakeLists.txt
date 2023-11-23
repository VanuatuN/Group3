# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(MD_SIMULATION)

project(MD_SIMULATION
    VERSION 1
    DESCRIPTION "Simulation"
    LANGUAGES C)

# Optionally set compiler options
set(CMAKE_CXX_FLAGS "-Wall")

include_directories(include)

# Define the sources for the library
set(LIB_SOURCES
    src/utilities.c
    src/compute_force.c
    src/input.c
    src/output.c
    src/cleanup.c
)

# Define the sources for the main executable
set(MAIN_SOURCES
    src/main.c
)

# Include the directory containing datastructure.h
include_directories(src)

# Create the library and include utilities.c
add_library(mdlib ${LIB_SOURCES})

# Create the main executable and link with the library
add_executable(ljmd_main ${MAIN_SOURCES})
target_link_libraries(ljmd_main PRIVATE mdlib)  # Use PRIVATE to avoid exporting dependencies of mdlib