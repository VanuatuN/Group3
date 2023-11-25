#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "datastructure.h"
#include "utilities.h"

void cleanup(FILE *erg, FILE *traj, mdsys_t *sys, double t_start){
    /* clean up: close files, free memory */
    printf("Simulation Done. Run time: %10.3fs\n", wallclock()-t_start);
    fclose(erg);
    fclose(traj);

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