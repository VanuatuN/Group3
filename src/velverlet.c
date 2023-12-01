#include <stdio.h>
#include <string.h>
#include <math.h>
#include "datastructure.h"
#include "utilities.h"
#include "compute_force.h"
#if defined(USE_MPI)
#include "mpi.h"
#endif

#if defined(USE_MPI)
#include "mpi.h"
#endif

#if defined(_OPENMP)
#include "omp.h"
#endif

// /* a few physical constants */
extern const double kboltz;     /* boltzman constant in kcal/mol/K */
extern const double mvsq2e; /* m*v^2 in kcal/mol */
void update_half1(mdsys_t *sys)
{ 
    /* first part: propagate velocities by half and positions by full step */
    for (int i = 0; i<sys->natoms; ++i) {
        sys->vx[i] += 0.5*sys->dt / mvsq2e * sys->fx[i] / sys->mass;
        sys->vy[i] += 0.5*sys->dt / mvsq2e * sys->fy[i] / sys->mass;
        sys->vz[i] += 0.5*sys->dt / mvsq2e * sys->fz[i] / sys->mass;
        sys->rx[i] += sys->dt*sys->vx[i];
        sys->ry[i] += sys->dt*sys->vy[i];
        sys->rz[i] += sys->dt*sys->vz[i];
    }
}

void update_half2(mdsys_t *sys)
{
    /* first part: propagate velocities by half and positions by full step */
    for (int i = 0; i<sys->natoms; ++i) {
        sys->vx[i] += 0.5*sys->dt / mvsq2e * sys->fx[i] / sys->mass;
        sys->vy[i] += 0.5*sys->dt / mvsq2e * sys->fy[i] / sys->mass;
        sys->vz[i] += 0.5*sys->dt / mvsq2e * sys->fz[i] / sys->mass;
    }
}

/* velocity verlet */
void velverlet(mdsys_t *sys)
{
    #if defined(_MPI)
    if (sys->rank == 0) {
    #endif
    /* first part: propagate velocities by half and positions by full step */
    update_half1(sys);
    #if defined(_MPI)
    }
    #endif

    /* compute forces and potential energy */
    #if defined(_MORSE)
        morse_force(sys);
    #else
        force(sys);
    #endif

    #if defined(_MPI)
    if (sys->rank == 0) {
    #endif
    update_half2(sys);
    #if defined(_MPI)
    }
    #endif
    /* second part: propagate velocities by another half step */
}

