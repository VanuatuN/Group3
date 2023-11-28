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
add_library(md_lib SHARED ${SOURCES} ${HEADERS})

# Specify include directories
target_include_directories(md_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create an executable
add_executable(md src/main.c)

# Enable debugging symbols
set(CMAKE_BUILD_TYPE Debug)

# Link the executable against the library
target_link_libraries(md PRIVATE md_lib m)

#Unit test for compute force function
option(ENABLE_TESTING "Enable building unit tests" ON)
if(ENABLE_TESTING)
  enable_testing()
  enable_language(CXX)

  find_package(GTest REQUIRED)
      # Download and include GoogleTest manually
      include(FetchContent)
      FetchContent_Declare(
          googletest
          URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
          DOWNLOAD_EXTRACT_TIMESTAMP true
      )
      FetchContent_MakeAvailable(googletest)

  set(TEST_FORCE
    tests/test_compute_force.cpp
  )
  add_executable(test_compute_force ${TEST_FORCE})
  target_include_directories(test_compute_force PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
  target_link_libraries(test_compute_force PUBLIC GTest::GTest GTest::Main md_lib)
  # add_test(NAME Force COMMAND test_compute_force)

  set(TEST_DSTRUCT
    tests/test_datastructure.cpp
  )

  add_executable(test_datastructure ${TEST_DSTRUCT})
  target_include_directories(test_datastructure PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
  target_link_libraries(test_datastructure PRIVATE GTest::GTest GTest::Main md_lib)
  add_test(NAME DataStructure COMMAND test_datastructure)
  
  set(TEST_SOURCES
    tests/test_velverlet.cpp
  )

  add_executable(test_velverlet ${TEST_SOURCES})
  target_include_directories(test_velverlet PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
  target_link_libraries(test_velverlet PRIVATE GTest::GTest GTest::Main md_lib)
  add_test(NAME Vervelet COMMAND test_velverlet)
  

  set(TEST_UTILS
      tests/test_utils.cpp
      )

  add_executable(test_utils ${TEST_UTILS})
  target_include_directories(test_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
  target_link_libraries(test_utils PUBLIC GTest::GTest GTest::Main md_lib)
  # add_test(NAME Utils COMMAND test_utils)
   # Add tests
   include(GoogleTest)
   gtest_discover_tests(test_utils TIMEOUT 1000)
   gtest_discover_tests(test_compute_force TIMEOUT 1000)
   # Specify the C++ standard
   set_target_properties(test_utils PROPERTIES CXX_STANDARD 14  CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS YES)
   set_target_properties(test_compute_force PROPERTIES CXX_STANDARD 14  CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS YES)
endif()
