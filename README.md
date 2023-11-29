## GROUP 3
- Edward  -  ed-ntim
- Jenny   -  jalmerol-mhpc
- Natalia -  VanuatuN 

# PROJECT:  LJ MD Model for Liquid Argon

## Overview

This repository provides a molecular dynamics (MD) model using the Lennard-Jones potential 
for simulating the behavior of liquid argon. The Lennard-Jones potential is a mathematical 
model commonly employed in molecular dynamics (MD) simulations to describe intermolecular interactions.

## Lennard-Jones Potential Equation

The Lennard-Jones potential is expressed by the following equation:

$\[ V(r) = 4 \epsilon \left[ \left(\frac{\sigma}{r}\right)^{12} - \left(\frac{\sigma}{r}\right)^6 \right] \]$

Where:
- $\( V(r) \)$ is the potential energy between two particles as a function of their separation distance $\( r \)$.
- $\( \epsilon \)$ is the depth of the potential well, representing the strength of the attractive forces between particles.
- $\( \sigma \)$ is the finite distance at which the inter-particle potential is zero, representing the distance at which 
the repulsive forces dominate.

The first term $\( \left(\frac{\sigma}{r}\right)^{12} \)$ represents the attractive forces, 
and the second term $\( \left(\frac{\sigma}{r}\right)^6 \)$ represents the repulsive forces.

## Usage

To utilize this model for liquid argon simulations, follow the instructions in the provided codebase. 
Make sure to adjust the values of $\( \epsilon \) and \( \sigma \)$ based on experimental data or 
quantum mechanical calculations to better represent the specific properties of argon.


## Simulation/Programming steps
1. Read in parameters and initial status and compute what is missing (e.g. accelerations)
2. Integrate Equations of motion with Velocity Verlet for a given number of steps
a) Propagate all velocities for half a step
b) Propagate all positions for a full step
c) Compute forces on all atoms to get accelerations
d) Propagate all velocities for half a step
e) Output intermediate results, if needed

## Expected output:
![Animation](ljmd.gif)\
Figure 1: Animation of the simulation of Argon (108 atoms) in a cubic box for 10000 MD steps

## Goals:

## I. Group Level

## II. Individual Level

## **********  Group 3 Directory Structure  **********
</pre>
```C
.<br>
├── CMakeLists.txt                ! CMake program for building the ljmd simulation program<br>
├── README.md                     ! Group & project description<br>
├── examples                      : Input files and their corresponding output files<br>
│   ├── argon_108.dat<br>
│   ├── argon_108.inp<br>
│   ├── argon_108.rest<br>
│   ├── argon_108.xyz<br>
│   ├── argon_2.rest<br>
│   ├── argon_2916.dat<br>
│   ├── argon_2916.inp<br>
│   ├── argon_2916.rest<br>
│   ├── argon_2916.xyz<br>
│   ├── argon_78732.inp<br>
│   └── argon_78732.rest<br>
├── include                        : Header files: contains function declarations<br>
│   ├── cleanup.h<br>
│   ├── compute_force.h<br>
│   ├── datastructure.h<br>
│   ├── input.h<br>
│   ├── output.h<br>
│   ├── utilities.h<br>
│   └── velverlet.h<br>
├── src                            : Source files : contains fucntion description<br>
│   ├── cleanup.c                  ! Memory deacllocation<br>
│   ├── compute_force.c            ! Force computation kernel<br>
│   ├── datastructure.c            ! Data types and structure definition<br>
│   ├── input.c                    ! Input algorithm<br>
│   ├── main.c                     ! Main program in C<br>
│   ├── main.cpp                   ! Main program in C++<br>
│   ├── output.c                   ! Data output algorithm<br>
│   ├── utilities.c                ! Helper functions (e.g get_a_line, pbc...)<br>
│   └── velverlet.c                ! Velocity Verlet computation kernel<br>
└── tests                          : Google Tests files<br>
    ├── test_compute_force.cpp     ! GTest (C++) program to check the correctness of the force computation kernel<br>
    ├── test_datastructure.cpp     ! GTest (C++) program to check the accuracy of data handling<br>
    ├── test_utils.cpp             ! GTest (C++) progrma to check helper functions used in the main program<br>
    └── test_velverlet.cpp         ! GTest (C++) program to check that velocities are computed correctly<br>
```
4 directories, 33 files
</pre>

## How to build (Serial code):

## How to build (With MPI parallelism):

## How to build (With OpenMP parallelism):



## Benchmark report (a):

## Benchmark report (b):

## Benchmark report ():

## Acknowledgments

This model is based on the widely accepted Lennard-Jones potential and may require further customization for specific applications. Please refer to relevant literature and consult experts in the field for a more accurate representation of liquid argon properties in your simulations.