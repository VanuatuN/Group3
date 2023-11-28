/**
 * Group 3 members:  Jenny, Natalia and Edward
*/

/*
 * simple lennard-jones potential MD code with velocity verlet.
 * units: Length=Angstrom, Mass=amu; Energy=kcal
 *
 * baseline c version.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "utilities.h"
#include "datastructure.h"
#include "input.h"
#include "output.h"
#include "compute_force.h"
#include "velverlet.h"

#if defined(_MPI)
    #include "mpi.h"
#endif

/* generic file- or pathname buffer length */
#define BLEN 200
#define LJMD_VERSION 1.0

typedef struct mdsys mdsys_t;

/* main */
int main(int argc, char **argv)
{
    int nprint, i, rank = 0, npes = 1;
    char restfile[BLEN], trajfile[BLEN], ergfile[BLEN], line[BLEN];
    FILE *fp,*traj,*erg;
    mdsys_t sys;
    double t_start;

    #if defined(_MPI)
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    sys.syscomm = MPI_COMM_WORLD;
    sys.rank = rank;
    sys.npes = npes;

    printf("rank %d", sys.rank);
    init_mdsys_mpi(&sys);

    if (rank == 0) {
    #endif
    printf("LJMD version %3.1f\n", LJMD_VERSION);
    t_start = wallclock();
    input(stdin, line, restfile, trajfile, ergfile, &sys, nprint, BLEN);
    init_mdsys(&sys); /* allocate memory */
    read_restart(&sys, restfile); /* read restart */
    
    /* initialize forces and energies.*/
    sys.nfi=0;
    #if defined(_MPI)
    } // if (rank == 0)
    #endif
    
    force(&sys); // ALL ranks

    #if defined(_MPI)
    if (rank == 0) {
    #endif

    ekin(&sys);
    erg=fopen(ergfile,"w");
    traj=fopen(trajfile,"w");

    printf("Startup time: %10.3fs\n", wallclock()-t_start);
    printf("Starting simulation with %d atoms for %d steps.\n",sys.natoms, sys.nsteps);
    printf("     NFI            TEMP            EKIN                 EPOT              ETOT\n");
    output(&sys, erg, traj);

    t_start = wallclock(); /* reset timer */

    #if defined(_MPI)
    } // if (rank == 0)
    #endif
    /**************************************************/
    /* main MD loop */
    for(sys.nfi=1; sys.nfi <= sys.nsteps; ++sys.nfi) {

        /* write output, if requested */
        if ((sys.nfi % nprint) == 0)
            output(&sys, erg, traj);

        /* propagate system and recompute energies */
        velverlet(&sys);
        ekin(&sys);
    }
    /**************************************************/

    #if defined(_MPI)
    if (rank == 0) {
    #endif
    /* clean up: close files, free memory */
    printf("Simulation Done. Run time: %10.3fs\n", wallclock()-t_start);
    fclose(erg);
    fclose(traj);

    cleanup_mdsys(&sys);

    #if defined(_MPI)
    } // if (rank == 0)
    #endif

    #if defined(_MPI)
    cleanup_mdsys_mpi(&sys);
    MPI_Finalize();
    #endif
    return 0;
}

