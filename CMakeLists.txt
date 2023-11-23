cmake_minimum_required(VERSION 3.10)

project(MD_SIMULATION)

# Add your source files here
set(SOURCES
    src/utilities.c
    src/compute_force.c
    src/input.c
    src/output.c
    src/cleanup.c
    src/main.c
    src/datastructure.c
)

# Add your header files here
set(HEADERS
    include/datastructure.h
    include/utilities.h
    include/compute_force.h
    include/input.h
    include/output.h
    include/cleanup.h
)

# Create an executable or library
add_executable(md ${SOURCES} ${HEADERS})

# Specify include directories
target_include_directories(md PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
