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

  set(TEST_SOURCES
    tests/test_datastructure.cpp
  )

  add_executable(test_datastructure ${TEST_SOURCES})
  target_include_directories(test_datastructure PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
  target_link_libraries(test_datastructure PRIVATE GTest::GTest GTest::Main md_lib)
  add_test(NAME DataStructure COMMAND test_datastructure)

endif()