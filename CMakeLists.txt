cmake_minimum_required(VERSION 3.12)
project(MD_SIMULATION LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)

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

# Create a library target for common code
add_library(md_lib SHARED ${SOURCES} ${HEADERS})

# Specify include directories
target_include_directories(md_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create an executable
add_executable(md src/main.c)

# Enable debugging symbols
set(CMAKE_BUILD_TYPE Debug)

# Link the executable against the library
target_link_libraries(md PRIVATE md_lib m)

option(ENABLE_TESTING "Enable building unit tests" ON)
if(ENABLE_TESTING)
    enable_testing()
    enable_language(CXX)

    # Download and include GoogleTest manually
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
    )
    FetchContent_MakeAvailable(googletest)

    set(TEST_SOURCES
        tests/test_utils.cpp
      )
    add_library(testlib SHARED ${TEST_SOURCES} ${HEADERS})
    add_executable(tests ${TEST_SOURCES} ${HEADERS})

    # Specify include directories
    target_include_directories(tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    # Link the executable against GTest and your library
    target_link_libraries(tests PRIVATE gtest gtest_main testlib md_lib m)
    # Add tests
    include(GoogleTest)
    gtest_discover_tests(tests TIMEOUT 1000)
    # Specify the C++ standard
    set_target_properties(tests PROPERTIES CXX_STANDARD 14  CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS YES)
endif()
