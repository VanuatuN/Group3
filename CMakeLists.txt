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

option(ENABLE_TESTING "Enable building unit tests" ON)
if(ENABLE_TESTING)
  enable_testing()
  enable_language(CXX)

  find_package(GTest REQUIRED)

  set(TEST_SOURCES
    tests/test_compute_force.cpp
  )
  add_executable(test_compute_force ${TEST_SOURCES})
  target_include_directories(test_compute_force PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
  target_link_libraries(test_compute_force PRIVATE GTest::GTest GTest::Main md_lib)
  add_test(NAME Force COMMAND test_compute_force)
endif()


# # Download and configure GoogleTest
# include(FetchContent)
# FetchContent_Declare(
#     googletest
#     URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
#     DOWNLOAD_EXTRACT_TIMESTAMP true
# )
# FetchContent_MakeAvailable(googletest)

# set(TEST_SOURCES
#     tests/test_compute_force.cpp
#     ${SOURCES} # Add this line if compute_force.c contains the implementation
# )

# add_executable(test_md ${TEST_SOURCES})

# # Include directories for the test target
# target_include_directories(test_md PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/src
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
# )

# target_link_libraries(test_md gtest_main)

# include(GoogleTest)
# # gtest_discover_tests(test_md TIMEOUT 1000)
# gtest_discover_tests(test_md TIMEOUT 300 PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DISCOVERY_TIMEOUT 60 TEST_DISCOVERY_TIMEOUT 60 TEST_RUN_TIMEOUT 300 TEST_RUN_OUTPUT_VARIABLE TEST_OUTPUT PARALLEL_LEVEL 4)
