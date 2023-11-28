#include <stdio.h>
#include <string.h>
#include <math.h>
#include "datastructure.h"
#include "utilities.h"

#if defined(USE_MPI)
#include "mpi.h"
#endif

/* compute forces */
void force(mdsys_t *sys)
{
    double c12 = 4.0 * sys->epsilon * pow(sys->sigma, 12.0);
    double c6 = 4.0 * sys->epsilon * pow(sys->sigma, 6.0);
    double rcsq = sys->rcut * sys->rcut;
    double ffac, rx, ry, rz, rsq;
    int step = 1;
    sys->epot = 0.0;
    
    #if defined(_MPI)
    int ii;
    double epotsum = 0;
    step = sys->npes;
    azzero(sys->cx, sys->natoms);
    azzero(sys->cy, sys->natoms);
    azzero(sys->cz, sys->natoms);

    MPI_Bcast(sys->rx, sys->natoms, MPI_DOUBLE, 0, sys->syscomm);
    MPI_Bcast(sys->ry, sys->natoms, MPI_DOUBLE, 0, sys->syscomm);
    MPI_Bcast(sys->rz, sys->natoms, MPI_DOUBLE, 0, sys->syscomm);

    #else
    azzero(sys->fx, sys->natoms);
    azzero(sys->fy, sys->natoms);
    azzero(sys->fz, sys->natoms);
    #endif
    
    for (int i = 0; i < (sys->natoms) - 1; i += step)
    {
        #if defined(_MPI)
        ii =  i + sys->rank;
        if (ii >=(sys->natoms-1)) break;
        #endif
        for (int j = i + 1; j < (sys->natoms); ++j)
        {
            rx = pbc(sys->rx[i] - sys->rx[j], 0.5 * sys->box);
            ry = pbc(sys->ry[i] - sys->ry[j], 0.5 * sys->box);
            rz = pbc(sys->rz[i] - sys->rz[j], 0.5 * sys->box);
            rsq = rx * rx + ry * ry + rz * rz;

            if (rsq < rcsq)
            {
                double r6, rinv;
                rinv = 1.0 / rsq;
                r6 = rinv * rinv * rinv;
                ffac = (12.0 * c12 * r6 - 6.0 * c6) * r6 * rinv;

                #if defined(_MPI)
                // Update forces using vectorized operations
                sys->cx[i] += rx * ffac;
                sys->cy[i] += ry * ffac;
                sys->cz[i] += rz * ffac;

                // Newton's third law: opposite force on particle j
                sys->cx[j] -= rx * ffac;
                sys->cy[j] -= ry * ffac;
                sys->cz[j] -= rz * ffac;

                epotsum += r6 * (c12 * r6 - c6);
                #else
                // Update forces using vectorized operations
                sys->fx[i] += rx * ffac;
                sys->fy[i] += ry * ffac;
                sys->fz[i] += rz * ffac;

                // Newton's third law: opposite force on particle j
                sys->fx[j] -= rx * ffac;
                sys->fy[j] -= ry * ffac;
                sys->fz[j] -= rz * ffac;
                sys->epot += r6 * (c12 * r6 - c6);
                #endif
            }
        }
    }
    #if defined(_MPI)
    MPI_Reduce(sys->cx, sys->fx, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->syscomm); 
    MPI_Reduce(sys->cy, sys->fy, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->syscomm); 
    MPI_Reduce(sys->cz, sys->fz, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->syscomm); 
    MPI_Reduce(&epotsum, &sys->epot, 1, MPI_DOUBLE, MPI_SUM, 0, sys->syscomm);
    #endif
}