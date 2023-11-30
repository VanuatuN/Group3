

## **********  Group 3 Directory Structure  **********
```C
.
├── CMakeLists.txt                     ! CMake program for building the ljmd simulation program
├── README.md                          ! Group & project description
├── examples                           : Input files and their corresponding output files
│   ├── argon_108.dat
│   ├── argon_108.inp
│   ├── argon_108.rest
│   ├── argon_108.xyz
│   ├── argon_2.rest
│   ├── argon_2916.dat
│   ├── argon_2916.inp
│   ├── argon_2916.rest
│   ├── argon_2916.xyz
│   ├── argon_78732.inp
│   └── argon_78732.rest
├── include                          : Header files: contains function declarations
│   ├── compute_force.h              
│   ├── datastructure.h
│   ├── input.h
│   ├── output.h
│   ├── utilities.h
│   └── velverlet.h
├── src                              : Source files : contains fucntion description
│   ├── compute_force.c              ! Force computation kernel
│   ├── datastructure.c              ! Data types and structure definition
│   ├── input.c                      ! Input algorithm
│   ├── main.c                       ! Main program in C
│   ├── output.c                     ! Data output algorithm
│   ├── utilities.c                  ! Helper functions (e.g get_a_line, pbc...)
│   └── velverlet.c                  ! Velocity Verlet computation kernel
└── tests                            : Google Tests files
    ├── test_compute_force.cpp       ! GTest (C++) program to check if force computation functions
    ├── test_datastructure.cpp       ! GTest (C++) program to check the accuracy of data handling
    ├── test_utils.cpp               ! GTest (C++) program to check helper functions used in the main program
    └── test_velverlet.cpp           ! GTest (C++) program to check that velocities are computed

4 directories, 30 files
```

