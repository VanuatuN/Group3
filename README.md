## GROUP 3
- Edward  -  ed-ntim
- Jenny   -  jalmerol-mhpc
- Natalia -  VanuatuN 

### PROJECT:  Lennard-Jones (LJ) MD Model for Simulating Liquid Argon

### Overview

This repository provides a molecular dynamics (MD) model using the Lennard-Jones potential 
for simulating the behavior of liquid argon. The Lennard-Jones potential is a mathematical 
model commonly used in molecular dynamics (MD) simulations to describe intermolecular interactions.

### Lennard-Jones Potential Equation

The Lennard-Jones potential is expressed by the following equation:

$\[ V(r) = 4 \epsilon \left[ \left(\frac{\sigma}{r}\right)^{12} - \left(\frac{\sigma}{r}\right)^6 \right] \]$


Where:
- $\( V(r) \)$ is the potential energy between two particles as a function of their separation distance $\( r \)$.
- $\( \epsilon \)$ is the depth of the potential well, representing the strength of the attractive forces between particles.
- $\( \sigma \)$ is the finite distance at which the inter-particle potential is zero, representing the distance at which 
the repulsive forces dominate.

The first term $\( \left(\frac{\sigma}{r}\right)^{12} \)$ represents the attractive forces, 
and the second term $\( \left(\frac{\sigma}{r}\right)^6 \)$ represents the repulsive forces.

### Usage

To use this model for liquid argon simulations, follow the instructions in the provided codebase. 
The values of $\( \epsilon \)$ and $\( \sigma \)$ can be adjusted based on experimental data or quantum mechanical calculations to better represent the specific properties of argon.


### Simulation/Programming steps
1. Read in parameters and initial status and compute what is missing (e.g. accelerations) <br>
2. Integrate Equations of motion with Velocity Verlet for a given number of steps <br>
a) Propagate all velocities for half a step <br>
b) Propagate all positions for a full step <br>
c) Compute forces on all atoms to get accelerations <br>
d) Propagate all velocities for half a step <br>
e) Output intermediate results, if needed <br> 

### Expected output:
![Animation](ljmd.gif)\
**Figure 1:** Animation of the simulation of Argon (108 atoms) in a cubic box for 10000 MD steps

### Goals:

**I. Group Level**\
To :
- break down the single file ljmd.c into multiple files (force compute, verlet time integration (split into two functions in one file), input, output, utilities, cleanup, main function, and header for data structures and prototypes); update the CMakeLists.txt file accordingly so that you build one executable from main.c and a library, mdlib.
- set up some simple unit tests with the googletest library (write C++ code that allocates/fills data structure(s), calls the respective functions and uses assertions to check if the result is as expected) and integrate those tests into the CMake procedure so the tests can be run with ctest.\
For example:\
a) compute forces for a few 2-3 particle systems with atoms inside/outside the cutoff (directly and with images via PBC)\
b) compute part of the time integration for given positions, forces and velocities (no call to force())\
c) compute kinetic energy for given velocities and mass\
d) create a minimal input file (containing a few atoms) and matching restart on the fly and verify that that data is read correctly

**II. Individual Level**\
a) Optimize the force computation: *refactor the code* for better optimization and to avoid costly operations or redundant work. Adapt data structures as needed. *Document improvements with profiling/benchmark data.*\
b) Add *MPI parallelization*. Document the *parallel efficiency* of changes.\
c) Add *OpenMP parallelization*. Document the *parallel efficiency* of changes

### How to build (Serial code):

To compile the default serial code with No optimizations, use the following commands:
```C
cmake -S . -B build
cmake --build build
cd build
ctest
```
- To compile with -O3 optimization, uncomment "# add_definitions(-O3)" in the 
"#Add Definitions" section of the CMakeLists.txt file, and repeat the compilation steps.

- To compile with Refactoring, uncomment "# add_definitions(-DREFACTOR)"
in the "#Add Definitions" section of the CMakeLists.txt file, and repeat the compilation steps.

- To compile with both -O3 and  Refactoring optimizations, uncomment both 
and repeat the compilation steps. 

### How to Build (With MPI parallelism):

### How to Build (With OpenMP parallelism):


### How to Run (Serial code):
```C
cd examples/
../build/./md < argon_108.inp > output.dat
more output.dat       
```
### How to Run (With MPI parallelism):

### How to Run (With OpenMP parallelism):

### Benchmark Report (a):

![Serial code with different optimizations](md_serial-1.png)<br>

**Figure 2:** Plot of the MD Runtimes(s) vs. Number of Atoms using different optimisation configuration on a MacBook, showing a general decrease in runtime with optimization.



![Alt text](speedupleo.png)
**Figure 3:** Plot of speedup of serial code with different optimization configurations (purple : -O3 compiler flag, green: Refactoring of force computation kernel, blue: combination of -O3 and refactoring) vs. Number of atoms (system size).
### Performance Analysis

**Table 1:** Summary of performance counter statistics for the LJMD code compiled with different optimization options for simulation of 108 Ar-atoms (best case)


| Metric                             | No Optimization | -O3 Optimization | Refactoring | -O3 Optimization + Refactoring |
|------------------------------------|-----------------|-------------------|-------------|--------------------------------|
| Cache References (K/sec)            | 1.579           | 22.723            | 74.305      | 155.732                        |
| Cache Misses (%)                    | 26.417          | 54.472            | 80.042      | 22.354                         |
| Instructions per Cycle             | 1.70            | 1.69              | 1.70        | 1.38                           |
| Branches (M/sec)                    | 869.835         | 939.936           | 617.361     | 897.081                        |
| Branch Misses (%)                   | 1.65            | 1.79              | 4.58        | 5.84                           |
| CPU Cycles (GHz)                    | 3.390           | 3.400             | 3.399       | 3.399                          |
| User Time (seconds)                 | 15.061          | 12.729            | 2.321       | 1.204                         |
| System Time (seconds)               | 0.004           | 0.005             | 0.002       | 0.004                          |
| Elapsed Time (seconds)              | 15.090          | 12.753            | 2.331       | 1.217                         |



**Cache Metrics (References and Misses):**

Influence: Cache references and misses are crucial for understanding memory access efficiency. The permutation of these metrics across different configurations indicates how well the code utilizes the cache hierarchy.
Observation: Cache references increase significantly with optimization, suggesting better cache utilization. The -O3 Optimization + Refactoring configuration stands out with both high references and low misses, indicating improved memory access patterns.

**Instructions per Cycle:**

Influence: Instructions per cycle reflect the efficiency of the code's execution. Lower values typically indicate better instruction-level parallelism and more efficient execution.
Observation: The instruction per cycle metric remains relatively stable across configurations. This suggests that, while optimizations impact other aspects, the efficiency of instruction execution is consistent.

**Branch Metrics (Branches and Branch Misses):**

Influence: Branches and branch misses provide insights into control flow efficiency. Optimizing branch prediction can significantly impact overall performance.
Observation: The number of branches executed per second increases with optimization, indicating potential changes in the control flow. However, the increase in branch misses in some configurations suggests that, while optimizations may improve overall execution speed, they could also introduce more challenging branch prediction scenarios.

**CPU Cycles:**

Influence: CPU cycles represent the total computational work done by the CPU. This metric is influenced by the efficiency of instruction execution and memory access patterns.
Observation: CPU cycles per second remain relatively consistent across all configurations, indicating that the optimizations do not significantly impact the overall computational workload.

**User Time, System Time, and Elapsed Time:**

Influence: User time reflects the time spent executing the code, while system time accounts for time spent in the operating system kernel. Elapsed time is the overall time taken for code execution.
Observation: User time decreases significantly with optimization, indicating improved overall runtime efficiency. The combination of -O3 optimization and refactoring provides the lowest user and system times, suggesting a balanced approach to optimization.

**Summary and Permutation of Metrics:**

Influence: The summary provides a comprehensive view of the trade-offs and benefits of different optimization configurations.
Observation: The permutation of metrics reveals that while some optimizations may improve certain aspects (e.g., cache efficiency), they could introduce challenges in others (e.g., increased branch misses). The combination of -O3 optimization and refactoring appears to strike a balance, resulting in the most favorable overall performance.
In conclusion, the permutation and combination of metrics offer a nuanced understanding of how different aspects of code optimization impact performance. It highlights the need for a holistic approach, considering the interplay of various metrics to achieve optimal results.

The worst-case scenario confirms the scalability of the LJMD code under different optimization configurations, with the larger dataset affecting certain metrics but maintaining similar trends observed in the smaller dataset.
The combination of -O3 optimization and refactoring continues to stand out as providing a balanced improvement in user and system times while maintaining reasonable cache misses and branch mispredictions, irrespective of the dataset size.
In summary, the LJMD code demonstrates scalability in its performance characteristics across different dataset sizes, and the optimization configurations continue to show consistent trends in improving efficiency, particularly in the -O3 optimization and refactoring combination.

### Benchmark Report (b):

### Benchmark Report (c):

### Acknowledgments

This model is based on the widely accepted Lennard-Jones potential and may require further customization for specific applications. Please refer to relevant literature and consult experts in the field for a more accurate representation of liquid argon properties in your simulations.