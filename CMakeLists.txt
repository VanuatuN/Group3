cmake_minimum_required(VERSION 3.6)

project(MD_SIMULATION)

# Add your source files here
set(SOURCES
    src/utilities.c
    src/compute_force.c
    src/velverlet.c
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
    include/velverlet.h
    include/compute_force.h
    include/input.h
    include/output.h
    include/cleanup.h
)

# Create an executable or library
add_executable(md ${SOURCES} ${HEADERS})

# Download and configure GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

set (TEST_SOURCES
    tests/test_compute_force.cpp
    )

add_executable(test_md ${TEST_SOURCES})

target_include_directories(test_md PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

target_link_libraries(test_md gtest_main)

include(GoogleTest)
gtest_discover_tests(test_md)

# Specify include directories
target_include_directories(md PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
