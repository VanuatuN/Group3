# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(MD_SIMULATION)

# Define the sources for the library
set(LIB_SOURCES
    utilities.c
    compute_force.c
    verlet.c
    input.c
    output.c
    cleanup.c
)

# Define the sources for the main executable
set(MAIN_SOURCES
    main.c
)

# Create the library
add_library(mdlib STATIC ${LIB_SOURCES})

# Create the main executable and link with the library
add_executable(ljmd_main ${MAIN_SOURCES})
target_link_libraries(ljmd_main PRIVATE mdlib)
