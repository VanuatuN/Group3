cmake_minimum_required(VERSION 3.12)  # Set a valid CMake version

project(MD_SIMULATION LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)

# Add your source files here
set(SOURCES
    src/utilities.c
    src/compute_force.c
    src/velverlet.c
    src/input.c
    src/output.c
    src/datastructure.c
)

# Add your header files here
set(HEADERS
    include/datastructure.h
    include/utilities.h
    include/velverlet.h
    include/compute_force.h
    include/input.h
    include/output.h
)

# Create a library target for common code
add_library(md_lib STATIC ${SOURCES} ${HEADERS})

# Specify include directories
target_include_directories(md_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create an executable
add_executable(md src/main.c)

# Link the executable against the library
target_link_libraries(md PRIVATE md_lib)