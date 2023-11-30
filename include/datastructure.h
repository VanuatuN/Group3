#ifndef C_DATASTRUCTURE_H
#define C_DATASTRUCTURE_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MPI)
#include "mpi.h"
#endif

#if defined(_OPENMP)
#include "omp.h"
#endif

/* a few physical constants */
extern const double kboltz; // = 0.0019872067;     /* boltzmann constant in kcal/mol/K */
extern const double mvsq2e; // = 2390.05736153349; /* m*v^2 in kcal/mol */

struct mdsys {
    int natoms, nfi, nsteps, nthreads, thid;
    double dt, mass, epsilon, sigma, box, rcut;
    double ekin, epot, temp;
    double *rx, *ry, *rz;
    double *vx, *vy, *vz;
    double *fx, *fy, *fz;
    #if defined(_MPI)
    double *cx, *cy, *cz;
    int rank;
    int npes;
    MPI_Comm syscomm;
    #endif
};
typedef struct mdsys mdsys_t;
void init_mdsys(mdsys_t *sys);
void cleanup_mdsys(mdsys_t *sys);
void read_restart(mdsys_t *sys, const char *restfile);

#if defined(_MPI)
   void init_mpi_c(mdsys_t *sys);
   void init_mpi_r(mdsys_t *sys);
   void cleanup_mpi_r(mdsys_t *sys);
   void cleanup_mpi_c(mdsys_t *sys);
   void init_params(mdsys_t *sys);
#endif

#ifdef __cplusplus
}
#endif

#endif
