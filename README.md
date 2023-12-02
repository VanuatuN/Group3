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

## Expected output:
<img src="ljmd1.gif" alt="animation" width="500" style="display: block; margin: auto;" /><br>


Figure 1: Animation of the simulation of Argon (108 atoms) in a cubic box for 10000 MD steps


### Goals:

**I. Group Level**\
To :
- break down the single file ljmd.c into multiple files (force compute, verlet time integration (split into two functions in one file), input, output, utilities, cleanup, main function, and header for data structures and prototypes); update the CMakeLists.txt file accordingly in order to build one executable from main.c and a library, mdlib.
- set up some simple unit tests with the googletest library (write C++ code that allocates/fills data structure(s), calls the respective functions and uses assertions to check if the result is as expected) and integrate the aforementioned tests into the CMake procedure so the tests can be run with ctest.\
For example:\
a) compute forces for a few 2-3 particle systems with atoms inside/outside the cutoff (directly and with images via PBC)\
b) compute part of the time integration for given positions, forces and velocities (no call to force())\
c) compute kinetic energy for given velocities and mass\
d) create a minimal input file (containing a few atoms) and matching restart on the fly and verify that that data is read correctly

**II. Individual Level**\
a) Edward: Optimize the force computation: *refactor the code* for better optimization and to avoid costly operations or redundant work. Adapt data structures as needed. *Document improvements with profiling/benchmark data.*<br> 
b) Jenny: Add *MPI parallelization*. Document the *parallel efficiency* of changes.<br>
c) Natalia: Add *OpenMP parallelization*. Document the *parallel efficiency* of changes. 

### Serial code:
Force Computation:

**Original Version of Force Kernel:**<br>
- Computes pairwise interactions in a nested loop.<br>
- Uses conditional checks to skip self-interactions.<br>
- Computes forces and energy with detailed expressions.<br>

**Optimized Version of Force Kernel (Refactoring & Newton's Third Law):**<br>
- Uses a more compact expression for force computation.
- Avoids self-interaction check by starting the inner loop from i + 1.<br>
- Uses constants computed outside the loop.<br>
- Uses vectorized operations to update forces for both particles simultaneously.<br>

**Serial code with refactoring optimization :**
The optimized version explicitly implements Newton's third law, updating forces for both particles involved in the interaction.
Also, energy accumulation is simplified.
The goal was to improve performance by simplifying expressions, eliminating redundant calculations, and taking advantage of vectorized operations. It maintains the same functionality as the original version but is more concise and potentially faster due to optimization techniques.

**Compiling Serial Code:**
To compile the default serial code, use the following commands:
```C
cmake -S . -B build -DENABLE_TESTING=ON
cmake --build build
cd build
ctest
```
with google tests turned on.

**Parallel (MPI) code:**
To compile the with MPI, use the following commands:
```C
cmake -S . -B build -DUSE_MPI=ON
cmake --build build
```

### How to Run:

```C
cd examples/
../build/./md < argon_108.inp > output.dat
more output.dat       
```

### Benchmark Report (a):
<img src="speedupleo.png" alt="animation" width="500" style="display: block; margin: auto;" /><br>
**Figure 1:** Plot of speedup of serial code with different optimization configurations (purple : -O3 compiler flag, green: Refactoring of force computation kernel, blue: combination of -O3 and refactoring) vs. Number of atoms (system size).
### Performance Analysis

**Table 1:** Summary of performance counter statistics for the LJMD code compiled with different optimization options for simulation of 108 Ar-atoms **(Best case)**

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

Cache references and misses help to understand memory access efficiency. Comparison of cache metrics across different configurations show how well the code utilizes the cache hierarchy.\
Cache references increase significantly with optimization, suggesting better cache utilization. The combination of "-O3" Optimization and Refactoring stands out with both high references and low misses, indicating improved memory access patterns.

**Instructions per Cycle:**

Instructions per cycle shows efficiency of the code's execution. Lower values typically indicate better instruction-level parallelism and more efficient execution. The instruction per cycle metric remains relatively stable across the different configurations. This suggests that, while configuration type affected other aspects of performance, the efficiency of instruction execution is consistent.

**Branch Metrics (Branches and Branch Misses):**

Branches and branch misses relate to control flow efficiency (for loops, if conditions, etc). The number of branches executed per second increased with optimization, indicating potential changes in the control flow. However, the increase in branch misses in some configurations suggests that, while optimizations may improve overall execution speed, they could possibly complicate branch prediction scenarios.

**CPU Cycles:**

CPU cycles represent the total computational work done by the CPU. This metric is influenced by the efficiency of instruction execution and memory access patterns.\
CPU cycles per second remain relatively consistent across all configurations, indicating that the optimizations do not significantly impact the overall computational workload.

**User Time, System Time, and Elapsed Time:**

User time reflects the time spent executing the code, while system time accounts for time spent in the cluster's OS kernel. Elapsed time is the overall time taken for code execution.
User time decreases significantly with optimization, indicating improved overall runtime efficiency. The combination of -O3 optimization and refactoring provides the lowest user and system times, suggesting a balanced approach to optimization.

**Worst Case Performance :**

The worst-case scenario (Ar-78732 atoms) confirmed the scalability of the LJMD code under different optimization configurations, with the larger dataset affecting certain metrics but maintaining similar trends observed in the smaller dataset (refer to perf_summary_78732.dat for details).
The combination of -O3 optimization and refactoring continued to stand out as providing a balanced improvement in user and system times while maintaining reasonable cache misses and branch mispredictions, irrespective of the dataset size.

**Summary :**

Though the serial code was generally inefficient with large system sizes, a good scaling with the best case provided insights into the trade-offs and benefits of different optimization configurations.
The permutation of metrics revealed that while some optimizations possibly improved certain hardware-level aspects (e.g., cache efficiency), they could introduce challenges in others (e.g., increased branch misses). The combination of -O3 optimization and refactoring appears to strike a balance, resulting in the most favorable overall performance.
In conclusion, the performance counters provided  a recognition and appreciation of the complexity, intricacies, and subtleties involved when adopting a configuration for optimization, rather than a simplistic or one-dimensional view of how different aspects of code optimization impact efficiency. It highlights the need for a holistic approach, considering the interplay of various metrics to achieve optimal results.

### MPI Parallelization:

A straightforward parallelization of the optimized serial code has been implemented, wherein the computation of forces is distributed across multiple processing elements. Initially, positions are broadcasted from rank 0. Subsequently, each rank calculates the force for different atoms. After the `compute_force()` operation, results are collected back to rank 0.

### Benchmark Report with MPI:

By integrating MPI parallelism into a refactored force kernel and utilizing the -O3 compiler flag for optimization, a consistent linear scaling was achieved with an increasing number of processes. For a system size of natoms = 108, the maximum speedup reaches only up to 7x when employing 8 processing elements. When utilizing more than 8 processing elements for 108 atoms, the communication overhead from the MPI calls becomes significant. After which the linear plot is observed to plateau. In contrast, for the worst-case scenario (natoms = 78732), the speedup linearly increases up to 96 tasks (refer to Figure 2). The size constraint of the serial code has been effectively eliminated with the incorporation of MPI parallelization.

<img src="mpi_speedup_plot.png" alt="animation" width="500" style="display: block; margin: auto;" /><br>

Figure 2: Speedup using MPI for different number of atoms.

### MPI+OpenMP Parallelization:

Another level of parallelization added by application of OpenMP inside of the each MPI process. OpenMP is used to take advantage of the shared memory inside the nodes. OpenMP is used to spawn N threads inside of the each MPI rank. The OpenMP Parallel directive is used inside the loop that calculates foces for each atom, inside the each MPI rank. So there are two metrics that can be applied to analyse the efficiency: 
- the speedup for the ration  N Threads/ N MPI ranks
- combination of the MPI ranks/ N Threads that gives the best speed up.  

When applying OpenMP we keep variables that declare indices and constants as private and firstprivate to protect
them from modification be all OpenMP threads and to avoid race conditions.

**Parallel (MPI+OpenMP) code:**

To compile the with MPI, use the following commands:
```C
cmake -S . -B build -DUSE_MPI=ON -DUSE_OPENMP=ON
cmake --build build
```
Leonardo has 2 NUMA nodes, each with 32 physical cores (supports multithreading, but we keep if off). In total there are 64 physical cores on Leonardo. So the optimal maximal configuration for total number of processes suggesrs that anks of MPI multiplied by threads of OpenMP (if we use both nodes) is 64 (e.g 2MPI*32 PpenMP, or 8 MPI *16 OpenMP) 

```C
Architecture:        x86_64
CPU op-mode(s):      32-bit, 64-bit
Byte Order:          Little Endian
CPU(s):              128
On-line CPU(s) list: 0-127
Thread(s) per core:  2
Core(s) per socket:  32
Socket(s):           2
NUMA node(s):        2
```
Each core has two threads. 
We run the program XX times with XX different inputs.
We use 1 node, so, 2 programs on each node.
Each program uses 6 MPI processes (12 per node).
Each process uses 3 threads
Therefore, each run uses 18 cores


### Benchmark Report with MPI+OpenMP:

We get a linear scaling in the speedup with just the simple parallelization of the compute_force() function for big problem size (as shown in the figure below). For natoms = 108, the maximum speedup

<img src="mpi_speedup_plot.png" alt="animation" width="500" style="display: block; margin: auto;" /><br>

Figure 3: Speedup using MPI+OpenMP for different number of atoms.

### Acknowledgments

- This model is based on the widely accepted Lennard-Jones potential and may require further customization for specific applications.

### References
- P1.7 Part-3 Lecture Slides, MHPC-2023, by Dr. Axel Kohlmeyer