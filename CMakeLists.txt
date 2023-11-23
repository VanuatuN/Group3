# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(MD_SIMULATION)

project(MD_SIMULATION
    VERSION 1
	DESCRIPTION "Simulation"
    LANGUAGES C)  # Specify the programming language (e.g., CXX for C++)

# Optionally set compiler options
set(CMAKE_CXX_FLAGS "-Wall")  # Set compiler flags (e.g., -Wall for C)


# Define the sources for the library
set(LIB_SOURCES
    src/utilities.c
    src/compute_force.c
    src/verlet.c
    src/input.c
    src/output.c
    src/cleanup.c
)

# Define the sources for the main executable
set(MAIN_SOURCES
    src/main.c
)

# Create the library
add_library(mdlib STATIC ${LIB_SOURCES})

# Create the main executable and link with the library
add_executable(ljmd_main ${MAIN_SOURCES})
target_link_libraries(ljmd_main PRIVATE mdlib)


default: check

clean:
	rm -f *.xyz *.dat

#check: ../ljmd-serial.x
#	../ljmd-serial.x < argon_108.inp
#	head -10 argon_108.dat | awk '{printf("%d %.6f %.6f %.6f\n",$$1,$$2,$$3,$$4);}'> a.dat
#	head -10 ../reference/argon_108.dat | awk '{printf("%d %.6f %.6f %.6f\n",$$1,$$2,$$3,$$4);}'> b.dat
#	cmp a.dat b.dat || exit 1
#	../ljmd-serial.x < argon_2916.inp
#	head -10 argon_2916.dat | awk '{printf("%d %.6f %.6f %.6f\n",$$1,$$2,$$3,$$4);}'> a.dat
#	head -10 ../reference/argon_2916.dat | awk '{printf("%d %.6f %.6f %.6f\n",$$1,$$2,$$3,$$4);}'> b.dat
#	cmp a.dat b.dat || exit 1
#	rm -f a.dat b.dat
#	../ljmd-serial.x < argon_78732.inp
#	head -10 argon_78732.dat | awk '{printf("%d %.6f %.6f %.6f\n",$$1,$$2,$$3,$$4);}'> a.dat
#	head -10 ../reference/argon_78732.dat | awk '{printf("%d %.6f %.6f %.6f\n",$$1,$$2,$$3,$$4);}'> b.dat
#	cmp a.dat b.dat || exit 1
#	rm -f a.dat b.dat

