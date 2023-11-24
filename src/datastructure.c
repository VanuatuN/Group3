// datastructure.c

#include "datastructure.h"
#include <stdlib.h>

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
