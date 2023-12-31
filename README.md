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

A straightforward parallelization of the optimized serial code has been implemented using MPI, where the computation of forces is distributed across multiple processing elements. The parallelization is implemented in the force kernel, wherein positions are initially broadcasted from rank 0 to all other ranks before computing the force. Then each rank calculates the force on different atoms. After the `compute_force()` operation, the results are collected back to rank 0.

**Parallel (MPI) code:**
To compile the code with MPI, use the following commands:
```C
cmake -S . -B build -DUSE_MPI=ON
cmake --build build
```

### Benchmark Report with MPI:

By integrating MPI parallelism into the refactored force kernel and utilizing the -O3 compiler flag for optimization, a consistent linear scaling was achieved with an increasing number of processes. For a system size of natoms = 108, the maximum speedup reaches only up to 7x when using 8 processing elements. When utilizing more than 8 processing elements for 108 atoms, the communication overhead from the MPI calls becomes significant. After which, the linear plot is observed to plateau. In contrast, for the worst-case scenario (natoms = 78732), the speedup linearly increases up to 96 tasks (refer to Figure 2). The size constraint of the serial code that has been previously observed has been effectively eliminated with the incorporation of MPI parallelization. 

<img src="mpi_speedup_plot.png" alt="animation" width="500" style="display: block; margin: auto;" /><br>

Figure 2: Speedup using MPI for different number of atoms.

Looking at the parallel efficiency in Figure 3, we can observe a good level of scalability for the case where natoms = 78732. In this scenario, the parallel efficiency consistently remains at or above 80%, indicating effective utilization of processing elements. This also suggests that the parallel algorithm scales well when solving problems with a larger number of atoms in the system.

Furthermore, our results show that favorable scalability for the case of natoms=108 is achieved if the number of processing elements does not exceed NPEs=8. Similarly, for natoms=2916, we achieve optimal scalability when employing NPEs=24. These findings underscore the importance of carefully selecting the number of processing elements based on the specific characteristics of the problem size. Maintaining a balance between computational resources and parallel efficiency is crucial for achieving optimal performance in parallel computing environments.

<img src="mpi_efficiency.png" alt="animation" width="500" style="display: block; margin: auto;" /><br>

Figure 3: MPI Parallel efficiency versus the number of processing elements for different number of atoms.

### MPI+OpenMP Parallelization:

Another level of parallelization added by application of OpenMP inside of the each MPI process. OpenMP is used to take advantage of the shared memory inside the nodes. OpenMP is used to spawn N threads inside of the each MPI rank. The OpenMP Parallel directive is used inside the loop that calculates foces for each atom, inside the each MPI rank. So there are two metrics that can be applied to analyse the efficiency: 
- the speedup for the ration  N Threads / N MPI ranks
- combination of the MPI ranks / N Threads that gives the best speed up.  

When applying OpenMP we keep variables that declare indices and constants as private and firstprivate to protect
them from modification be all OpenMP threads and to avoid race conditions.

**Parallel (MPI+OpenMP) code:**

To compile the with MPI, use the following commands:
```C
cmake -S . -B build -DUSE_MPI=ON -DUSE_OPENMP=ON
cmake --build build
```
Leonardo has 2 NUMA nodes, each with 32 physical cores (supports multithreading, but we keep if off). In total there are 64 physical cores on Leonardo. So the optimal maximal configuration for total number of processes suggests that max ranks of MPI multiplied by max threads of OpenMP (if we use both nodes) is 64 (e.g 2MPI * 32OpenMP, or 8 MPI * 16 OpenMP) 

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

We submit sbatch scripts with the following structure that runs all combinations of the number of MPI processes and number of OpenMP ranks, we keep the total processes below 32 to satisfy the size of the Leonardo, we also use *--cpu_bind=cores* option to bind MPI processes to specific CPU cores:

```C
#!/bin/bash
#SBATCH --job-name=benchmark_test
#SBATCH --output=benchmark_test.out
#SBATCH --error=benchmark_test.err
#SBATCH --nodes=1
#SBATCH --cpus-per-task=32
#SBATCH -A ICT23_MHPC
#SBATCH --time=00:30:00
#SBATCH --partition=boost_usr_prod

# Define the number of MPI processes and OpenMP threads
MPI_PROCS=(1 2 4 8 16 32)
OMP_THREADS=(32 16 8 4 2 1)
atom=78732

for MPI_PROC in "${MPI_PROCS[@]}"; do
  for OMP_THREAD in "${OMP_THREADS[@]}"; do
    output_file="${atom}-benchmark-openmp+mpi.out"
    export OMP_NUM_THREADS=$OMP_THREAD
    srun -n $MPI_PROC -c $OMP_THREAD --cpu_bind=cores  ./md <argon_${atom}.inp >> "${output_file}" 
  done
done
```

We test different configurations of tasks per node and cpu cores per task to achieve the best performance. Example script uses 1 node and combinations of (1 2 4 8 16 32) MPI processes together with (32 16 8 4 2 1) OpenMP threads keeping the total number below 32. 

Note, that without specification of the option *--cpu_bind=cores* OpenMP nad MPI do not provide improvement in performance, rather makes running time higher, likely due to communication time and nonuniform (among cores) memory distribution. 


### Benchmark Report with MPI+OpenMP:

### 108 atoms

For the system size 108 atoms the combination of MPI and OpenMP do not provide any meaningful results, as likely system size is too small to be distributed properly across cores. Usage of 1 OPENMP thread and diffent number of MPI tasks results in the following speedup comparing to the serial run:

<img src="speedup_plot_108.png" alt="animation" width="500" style="display: block; margin: auto;" /><br>

**Figure 4:**  Speedup using MPI+OpenMP for system of 108 atoms size.

The speedup makes drop after 16 MPI ranks likely due to the communication time between processes and separation of the array across multiple processes.

### 78732 atoms 

The simulations were run with 78732 atoms for 20 steps. The startup time varies with the number of threads used.

The subsequent runs follow a similar pattern with different combinations of npes and nthreads. The run times generally increase as the number of processing elements or threads decreases.

<img src="speedup_comparison_78732.png" alt="animation" width="500" style="display: block; margin: auto;" /><br>

**Figure 5:**  Speedup using MPI+OpenMP for system of 78732 atoms size.


<img src="execution_startup_time_comparison_78732.png" alt="animation" width="500" style="display: block; margin: auto;" /><br>

**Figure 6:**  Execution and startup times using MPI+OpenMP for system of 78732 atoms size.


***Startup Time:***

Startup time appears to be relatively consistent for a given configuration (npes and nthreads). It is expected that startup time remains relatively stable since it is often related to initializing the simulation environment and data structures.

***Simulation Time:***

As expected, simulation time increases as the number of processing elements (npes) decreases, reflecting the parallel nature of the simulation. Fewer npes mean fewer parallel processes, leading to longer simulation times.

***Impact of Threads:***

With a fixed number of processing elements (npes), the simulation time increases as the number of threads decreases. This is consistent with the typical behavior of parallel simulations, where fewer threads result in less parallelism and longer runtimes.

***Overall Performance:***

The fastest simulation is achieved with the highest level of parallelism (npes and threads). For example, the simulation with npes=1 and nthreads=32 has the lowest total runtime (18.060 seconds).

***Speedup:***

The optimal configuration is the one that achieves the highest speedup. In this case, the configuration with npes 16 and nthreads 2 provides the highest speedup, however the differences are very small. This configuration strikes a balance between the number of processing elements and threads, resulting in the best overall performance for the given simulation workload.

### 2916 atoms 

The optimal configuration for the LJMD simulation with a system size of 2916 atoms is npes 32 and nthreads 1, however the speedup gains diminish as the number of processing elements or threads increases, indicating potential bottlenecks or overhead. 

<img src="speedup_comparison_2916.png" alt="animation" width="500" style="display: block; margin: auto;" /><br>

**Figure 7:**  Speedup using MPI+OpenMP for system of 2916 atoms size.


<img src="execution_startup_time_comparison_2916.png" alt="animation" width="500" style="display: block; margin: auto;" /><br>

**Figure 8:**  Execution and startup times using MPI+OpenMP for system of 2916 atoms size.


### Conclusions

The system size plays critical role for the scalability of the task on Leonardo. For the largest system size of 78732 atoms the scalability of the most prominent and demostrates hish sensitivity the configuration of MPI npes and OPENMP nthreads. 

### Extra:

- The code can be run using Morse potentential instead of Lennard-Jones potential. Follow the [link](https://github.com/VanuatuN/Group3/blob/development/morse_potential.md) on how to run the progam with Morse potential.

- A python wrapper has been implemented so the mdlib library can be called from python. The python main can be found [here](https://github.com/VanuatuN/Group3/blob/development/python/ljmd-python.py). You can find instruction on how to run the program [here](https://github.com/VanuatuN/Group3/blob/development/python/readme.md).

### Acknowledgments

- This model is based on the widely accepted Lennard-Jones potential and may require further customization for specific applications.

### References
- P1.7 Part-3 Lecture Slides, MHPC-2023, by Dr. Axel Kohlmeyer