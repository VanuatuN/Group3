#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <chrono>
#include "../include/headers.h"

/* generic file- or pathname buffer length */
#define BLEN 200
#define LJMD_VERSION 1.0

/* main */
int main(int argc, char **argv) {
    int nprint, i;
    char restfile[BLEN], trajfile[BLEN], ergfile[BLEN], line[BLEN];
    std::ifstream fp;
    std::ofstream traj, erg;
    mdsys_t sys;
    double t_start;

    std::cout << "LJMD version " << LJMD_VERSION << std::endl;

    t_start = wallclock();

    input(std::cin, line, restfile, trajfile, ergfile);

    /* allocate memory */
    sys.rx = new double[sys.natoms];
    sys.ry = new double[sys.natoms];
    sys.rz = new double[sys.natoms];
    sys.vx = new double[sys.natoms];
    sys.vy = new double[sys.natoms];
    sys.vz = new double[sys.natoms];
    sys.fx = new double[sys.natoms];
    sys.fy = new double[sys.natoms];
    sys.fz = new double[sys.natoms];

    /* read restart */
    fp.open(restfile);
    if (fp.is_open()) {
        for (i = 0; i < sys.natoms; ++i) {
            fp >> sys.rx[i] >> sys.ry[i] >> sys.rz[i];
        }
        for (i = 0; i < sys.natoms; ++i) {
            fp >> sys.vx[i] >> sys.vy[i] >> sys.vz[i];
        }
        fp.close();
        azzero(sys.fx, sys.natoms);
        azzero(sys.fy, sys.natoms);
        azzero(sys.fz, sys.natoms);
    } else {
        std::cerr << "cannot read restart file" << std::endl;
        return 3;
    }

    /* initialize forces and energies. */
    sys.nfi = 0;
    force(&sys);
    ekin(&sys);

    erg.open(ergfile);
    traj.open(trajfile);

    std::cout << "Startup time: " << wallclock() - t_start << "s\n";
    std::cout << "Starting simulation with " << sys.natoms << " atoms for " << sys.nsteps << " steps.\n";
    std::cout << "     NFI            TEMP            EKIN                 EPOT              ETOT\n";
    output(&sys, erg, traj);

    /* reset timer */
    t_start = wallclock();

    /**************************************************/
    /* main MD loop */
    for (sys.nfi = 1; sys.nfi <= sys.nsteps; ++sys.nfi) {

        /* write output, if requested */
        if ((sys.nfi % nprint) == 0)
            output(&sys, erg, traj);

        /* propagate system and recompute energies */
        velverlet(&sys);
        ekin(&sys);
    }
    /**************************************************/

    cleanup(erg, traj, sys);

    // Clean up dynamically allocated memory
    delete[] sys.rx;
    delete[] sys.ry;
    delete[] sys.rz;
    delete[] sys.vx;
    delete[] sys.vy;
    delete[] sys.vz;
    delete[] sys.fx;
    delete[] sys.fy;
    delete[] sys.fz;

    return 0;
}
