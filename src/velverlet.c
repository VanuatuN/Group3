#include <stdio.h>
#include <string.h>
#include <math.h>
#include "datastructure.h"
#include "utilities.h"
#include "compute_force.h"

void update_half1(mdsys_t *sys, int i)
{
    /* first part: propagate velocities by half and positions by full step */
    for (i=0; i<sys->natoms; ++i) {
        sys->vx[i] += 0.5*sys->dt / mvsq2e * sys->fx[i] / sys->mass;
        sys->vy[i] += 0.5*sys->dt / mvsq2e * sys->fy[i] / sys->mass;
        sys->vz[i] += 0.5*sys->dt / mvsq2e * sys->fz[i] / sys->mass;
        sys->rx[i] += sys->dt*sys->vx[i];
        sys->ry[i] += sys->dt*sys->vy[i];
        sys->rz[i] += sys->dt*sys->vz[i];
    }
}

void update_half2(mdsys_t *sys, int i)
{
    /* first part: propagate velocities by half and positions by full step */
    for (i=0; i<sys->natoms; ++i) {
        sys->vx[i] += 0.5*sys->dt / mvsq2e * sys->fx[i] / sys->mass;
        sys->vy[i] += 0.5*sys->dt / mvsq2e * sys->fy[i] / sys->mass;
        sys->vz[i] += 0.5*sys->dt / mvsq2e * sys->fz[i] / sys->mass;
//         sys->rx[i] += sys->dt * sys->vx[i];
//         sys->ry[i] += sys->dt * sys->vy[i];
//         sys->rz[i] += sys->dt * sys->vz[i];
    }
}

/* velocity verlet */
void velverlet(mdsys_t *sys)
{
    int i;

    /* first part: propagate velocities by half and positions by full step */
    update_half1(sys, i);

    /* compute forces and potential energy */
    force(sys);

    update_half2(sys, i);

    /* second part: propagate velocities by another half step */
    
}

