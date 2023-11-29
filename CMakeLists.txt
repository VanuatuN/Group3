cmake_minimum_required(VERSION 3.12)

project(MD_SIMULATION LANGUAGES C CXX)

option(USE_MPI "Use MPI for compilation" OFF)
option(USE_OPENMP "Use OpenMP for compilation" OFF)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

if(USE_MPI)
    find_package(MPI REQUIRED)
    message(STATUS "MPI include path: ${MPI_C_INCLUDE_PATH}")
    message(STATUS "MPI libraries: ${MPI_C_LIBRARIES}")
    set(CMAKE_C_COMPILER ${MPI_C_COMPILER})
    include_directories(SYSTEM ${MPI_C_INCLUDE_PATH})
    add_definitions(-D_MPI)
    set(MPI_LIBRARIES "${MPI_C_LIBRARIES}")
else()
    set(CMAKE_C_COMPILER gcc)
endif()


if(USE_OPENMP)
    find_package(OpenMP)
    message(STATUS "OpenMP include path: ${OpenMP_C_INCLUDE_PATH}")
    set(CMAKE_C_COMPILER ${OpenMP_C_COMPILER})
    include_directories(SYSTEM ${OpenMP_C_INCLUDE_PATH})
    add_definitions(-D_OPENMP)
    set(OpenMP_LIBRARIES "${OpenMP_C_LIBRARIES}")
else()
    set(CMAKE_C_COMPILER gcc)
endif()


add_definitions(-O3)

set(SOURCES
    src/utilities.c
    src/compute_force.c
    src/velverlet.c
    src/input.c
    src/output.c
    src/datastructure.c
)

set(HEADERS
    include/datastructure.h
    include/utilities.h
    include/velverlet.h
    include/compute_force.h
    include/input.h
    include/output.h
)

add_library(md_lib SHARED ${SOURCES} ${HEADERS})
target_include_directories(md_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(md src/main.c)
target_link_libraries(md PRIVATE md_lib m)

if(USE_MPI)
    target_include_directories(md_lib SYSTEM PRIVATE ${MPI_C_INCLUDE_PATH})
    target_include_directories(md SYSTEM PRIVATE ${MPI_C_INCLUDE_PATH})
    target_compile_definitions(md PRIVATE _MPI)
endif()

if(USE_OPENMP)
    target_include_directories(md_lib SYSTEM PRIVATE ${OpenMP_C_INCLUDE_PATH})
    target_include_directories(md SYSTEM PRIVATE ${OpenMP_C_INCLUDE_PATH})
    target_compile_definitions(md PRIVATE _OPENMP)
endif()    

#Unit tests
option(ENABLE_TESTING "Enable building unit tests" ON)
if(ENABLE_TESTING)
  enable_testing()
  enable_language(CXX)

  find_package(GTest REQUIRED)

  set(TEST_FORCE
    tests/test_compute_force.cpp
  )
  add_executable(test_compute_force ${TEST_FORCE})
  target_include_directories(test_compute_force PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
  target_link_libraries(test_compute_force PRIVATE GTest::GTest GTest::Main md_lib)
  add_test(NAME Force COMMAND test_compute_force)

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
  target_include_directories(test_utils PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
  target_link_libraries(test_utils PRIVATE GTest::GTest GTest::Main md_lib)
  add_test(NAME Utils COMMAND test_utils)
endif()
