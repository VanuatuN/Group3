cmake_minimum_required(VERSION 3.12)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

project(MD_SIMULATION LANGUAGES C CXX)

option(USE_MPI "Use MPI for compilation" ON)
option(USE_OpenMP "Enable OpenMP" ON)

#set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
#set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
set(OPENMP_LIBRARIES "/opt/homebrew/opt/llvm/lib")
set(OPENMP_INCLUDES "/opt/homebrew/opt/llvm/include")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


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


# Find OpenMP
if(APPLE AND USE_OpenMP)
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp")
        set(OpenMP_C_LIB_NAMES "omp")
        set(OpenMP_omp_LIBRARY omp)
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp")
        set(OpenMP_CXX_LIB_NAMES "omp")
        set(OpenMP_omp_LIBRARY omp)
    endif()
endif()

if(USE_OpenMP)
  find_package(OpenMP REQUIRED)
  message(STATUS "OpenMP include path: ${OPENMP_LIBRARIES}")
  message(STATUS "OpenMP libraries: ${OpenMP_C_LIBRARIES}")
endif(USE_OpenMP)


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



#    target_link_libraries(md PUBLIC OpenMP::OpenMP_C)
#    if(CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
#     target_compile_options(md PUBLIC -Xclang -fopenmp)
#    else()
#     target_compile_options(md PUBLIC -fopenmp) 
#    endif()

#endif()     

if(USE_OpenMP)
    target_include_directories(md_lib SYSTEM PRIVATE ${OPENMP_INCLUDES})
    target_include_directories(md SYSTEM PRIVATE ${OPENMP_INCLUDES})
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
