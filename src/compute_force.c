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
    double rx, ry, rz, rsq, ffac;

    /* zero energy and forces */
    sys->epot = 0.0;
    azzero(sys->fx, sys->natoms);
    azzero(sys->fy, sys->natoms);
    azzero(sys->fz, sys->natoms);

    for (int i = 0; i < (sys->natoms) - 1; ++i)
    {
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

                // sys->epot += r6 * (c12 * r6 - c6);

                // Update forces using vectorized operations
                sys->fx[i] += rx * ffac;
                sys->fy[i] += ry * ffac;
                sys->fz[i] += rz * ffac;

                // Newton's third law: opposite force on particle j
                sys->fx[j] -= rx * ffac;
                sys->fy[j] -= ry * ffac;
                sys->fz[j] -= rz * ffac;

                sys->epot += r6 * (c12 * r6 - c6);
                
            }
        }
    }
}

// /* compute forces */
// void force(mdsys_t *sys)
// {
//     double r,ffac;
//     double rx,ry,rz;
//     int i,j, ii;
//     int step = 1;

//     #if defined(_MPI)
//     if (rank ==0) {
//     #endif

//     /* zero energy and forces */
//     sys->epot=0.0;
//     azzero(sys->fx,sys->natoms);
//     azzero(sys->fy,sys->natoms);
//     azzero(sys->fz,sys->natoms);

//     #if defined(_MPI)
//     } // end if (rank == 0)
//     #endif

//     #if defined(_MPI)
//     step = npes;

//     MPI_Bcast(sys->rx, sys->natoms, MPI_DOUBLE, 0, sys->syscomm); 
//     MPI_Bcast(sys->ry, sys->natoms, MPI_DOUBLE, 0, sys->syscomm); 
//     MPI_Bcast(sys->rz, sys->natoms, MPI_DOUBLE, 0, sys->syscomm);
//     #endif

//     for (i=0; i < sys->natoms-1; i += step) {
//     #if defined(_MPI)
//     ii = i + sys->rank;
//     #else
//     ii = i;
//     #endif
//     // if (ii >= (sys->natoms - 1)) break;
//     for (j=i+1; i < sys->natoms; ++j) {
//     /* particles have no interactions with themselves */
//             if (i==j) continue;

//             /* get distance between particle i and j */
//             rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
//             ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
//             rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
//             r = sqrt(rx*rx + ry*ry + rz*rz);

//             /* compute force and energy if within cutoff */
//             if (r < sys->rcut) {
//                 ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r,12.0)/r+6*pow(sys->sigma/r,6.0)/r);

//                 sys->epot += 0.5*4.0*sys->epsilon*(pow(sys->sigma/r,12.0)-pow(sys->sigma/r,6.0));

//                 #if defined(_MPI)
//                 sys->cx[i] += rx/r*ffac;
//                 sys->cy[i] += ry/r*ffac;
//                 sys->cz[i] += rz/r*ffac;
//                 #else
//                 sys->fx[i] += rx/r*ffac;
//                 sys->fy[i] += ry/r*ffac;
//                 sys->fz[i] += rz/r*ffac;
//                 #endif
//             }
//         }
//     }

//     #if defined(_MPI)
//     MPI_Reduce(sys->cx, sys->fx, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm); 
//     MPI_Reduce(sys->cy, sys->fy, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm); 
//     MPI_Reduce(sys->cz, sys->fz, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm); 
//     MPI_Reduce(&epot, &sys->epot, 1, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm);
//     #endif
// }