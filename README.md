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
├── CMakeLists.txt                &nbsp; ! CMake program for building the ljmd simulation program
├── README.md                     &nbsp; ! Group & project description
├── examples                      &nbsp; : Input files and their corresponding output files
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
├── include                       &nbsp; : Header files: contains function declarations
│   ├── cleanup.h
│   ├── compute_force.h
│   ├── datastructure.h
│   ├── input.h
│   ├── output.h
│   ├── utilities.h
│   └── velverlet.h
├── src                           &nbsp;  : Source files : contains function description
│   ├── cleanup.c                  &nbsp; ! Memory deallocation
│   ├── compute_force.c            &nbsp; ! Force computation kernel
│   ├── datastructure.c            &nbsp; ! Data types and structure definition
│   ├── input.c                    &nbsp; ! Input algorithm
│   ├── main.c                     &nbsp; ! Main program in C
│   ├── main.cpp                   &nbsp; ! Main program in C++
│   ├── output.c                   &nbsp; ! Data output algorithm
│   ├── utilities.c                &nbsp; ! Helper functions (e.g., get_a_line, pbc...)
│   └── velverlet.c                &nbsp; ! Velocity Verlet computation kernel
└── tests                        &nbsp;   : Google Tests files
    ├── test_compute_force.cpp     &nbsp; ! GTest (C++) program to check the correctness of the force computation kernel
    ├── test_datastructure.cpp     &nbsp; ! GTest (C++) program to check the accuracy of data handling
    ├── test_utils.cpp             &nbsp; ! GTest (C++) program to check helper functions used in the main program
    └── test_velverlet.cpp         &nbsp; ! GTest (C++) program to check that velocities are computed correctly

4 directories, 33 files

## How to build (Serial code):

## How to build (With MPI parallelism):

## How to build (With OpenMP parallelism):



## Benchmark report (a):

## Benchmark report (b):

## Benchmark report (c):

## Acknowledgments

This model is based on the widely accepted Lennard-Jones potential and may require further customization for specific applications. Please refer to relevant literature and consult experts in the field for a more accurate representation of liquid argon properties in your simulations.