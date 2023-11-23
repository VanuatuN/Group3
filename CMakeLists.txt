cmake_minimum_required(VERSION 3.10)
project(MD_SIMULATION
    VERSION 1
	DESCRIPTION "Simulation"
    LANGUAGES C)  # Specify the programming language (e.g., CXX for C++)

# Optionally set compiler options
set(CMAKE_CXX_FLAGS "-Wall")  # Set compiler flags (e.g., -Wall for C)

# Include directories
include_directories(include)

# Define the sources for the library
# Define the sources for the library
set(LIB_SOURCES
    src/utilities.c
    src/compute_force.c
    src/input.c
    src/output.c
    src/cleanup.c
)

# Create the library
add_library(mdlib ${LIB_SOURCES})

# Define the sources for the main executable
set(MAIN_SOURCES
    src/main.c
)

# Create the main executable and link with the library
add_executable(ljmd_main ${MAIN_SOURCES})
target_link_libraries(ljmd_main PUBLIC mdlib)