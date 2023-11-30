// datastructure.c
#include <stdio.h>
#include "datastructure.h"
#include <stdlib.h>
#include "utilities.h"
#include <math.h>

#if defined(_MPI)
#include "mpi.h"
#endif


#if defined(_OPENMP)
#include "omp.h"
#endif

// Function to initialize the mdsys_t structure
void init_mdsys(mdsys_t *sys) {
    sys->rx = (double *)malloc(sys->natoms * sizeof(double));
    sys->ry = (double *)malloc(sys->natoms * sizeof(double));
    sys->rz = (double *)malloc(sys->natoms * sizeof(double));
    sys->vx = (double *)malloc(sys->natoms * sizeof(double));
    sys->vy = (double *)malloc(sys->natoms * sizeof(double));
    sys->vz = (double *)malloc(sys->natoms * sizeof(double));
    sys->fx = (double *)malloc(sys->natoms * sizeof(double));
    sys->fy = (double *)malloc(sys->natoms * sizeof(double));
    sys->fz = (double *)malloc(sys->natoms * sizeof(double));
}

#if defined(_MPI)
void init_mpi_c(mdsys_t *sys) {
    sys->cx = (double *)malloc(sys->natoms * sizeof(double));
    sys->cy = (double *)malloc(sys->natoms * sizeof(double));
    sys->cz = (double *)malloc(sys->natoms * sizeof(double));
}

void init_mpi_r(mdsys_t *sys) {
    sys->rx = (double *)malloc(sys->natoms * sizeof(double));
    sys->ry = (double *)malloc(sys->natoms * sizeof(double));
    sys->rz = (double *)malloc(sys->natoms * sizeof(double));
}

void cleanup_mpi_c(mdsys_t *sys){
    free(sys->cx);
    free(sys->cy);
    free(sys->cz);
}

void cleanup_mpi_r(mdsys_t *sys){
    free(sys->rx);
    free(sys->ry);
    free(sys->rz);
}

void init_params(mdsys_t *sys){
    MPI_Bcast(&sys->natoms, 1, MPI_INT, 0, sys->syscomm);
    MPI_Bcast(&sys->box, 1, MPI_DOUBLE, 0, sys->syscomm);
    MPI_Bcast(&sys->rcut, 1, MPI_DOUBLE, 0, sys->syscomm);
    MPI_Bcast(&sys->epsilon, 1, MPI_DOUBLE, 0, sys->syscomm);
    MPI_Bcast(&sys->sigma, 1, MPI_DOUBLE, 0, sys->syscomm);
    MPI_Bcast(&sys->nsteps, 1, MPI_INT, 0, sys->syscomm);
    MPI_Bcast(&sys->dt, 1, MPI_DOUBLE, 0, sys->syscomm);
}
#endif

// Function to free resources associated with the mdsys_t structure
void cleanup_mdsys(mdsys_t *sys) {
    // Free memory for arrays
    free(sys->rx);
    free(sys->ry);
    free(sys->rz);
    free(sys->vx);
    free(sys->vy);
    free(sys->vz);
    free(sys->fx);
    free(sys->fy);
    free(sys->fz);
}

void read_restart(mdsys_t *sys, const char *restfile){
    /* read restart */
    int i;
    FILE *fp;
    fp=fopen(restfile,"r");
    if(fp) {
        for (i=0; i<sys->natoms; ++i) {
            fscanf(fp,"%lf%lf%lf",sys->rx+i, sys->ry+i, sys->rz+i);
        }
        for (i=0; i<sys->natoms; ++i) {
            fscanf(fp,"%lf%lf%lf",sys->vx+i, sys->vy+i, sys->vz+i);
        }
        fclose(fp);
        azzero(sys->fx, sys->natoms);
        azzero(sys->fy, sys->natoms);
        azzero(sys->fz, sys->natoms);
    } else {
        perror("cannot read restart file");
        // return 3;
    }
}
