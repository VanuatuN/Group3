#include <stdio.h>
#include <string.h>
#include <math.h>
#include "datastructure.h"
#include "utilities.h"

#if defined(_MPI)
#include "mpi.h"
#endif

#if defined(_OPENMP)
#include "omp.h"
#endif

/* compute forces */

void force(mdsys_t *sys)
{   
    #if defined(_MORSE)
    double exponent = 1.0 /6.0;
    double R = pow(2.0, exponent) * sys->sigma;
    double alpha = 6.0 / R;
    double coeffs = 2.0 * sys->epsilon * alpha;
    #else
    double c12 = 4.0 * sys->epsilon * pow(sys->sigma, 12.0);
    double c6 = 4.0 * sys->epsilon * pow(sys->sigma, 6.0);
    #endif
    double rcsq = sys->rcut * sys->rcut;
    double ffac, rx, ry, rz, rsq, r6, rinv, epot=0.0;
    int step = 1;
    sys->epot = 0.0;
    int ii, i, j;
    int thid;
    
    #if defined(_MPI)
    double epotsum = 0.0;
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

    #if defined(_OPENMP)
    #if defined(_MORSE)
    #pragma omp parallel private(i, j, ii, rx, ry, rz, rsq, ffac,thid) firstprivate(exponent, R, alpha, coeffs) reduction(+:epot) 
    #else
    #pragma omp parallel private(i, j, ii, rx, ry, rz, rsq, ffac, r6, rinv,thid) firstprivate(c12, c6, rcsq) reduction(+:epot) 
    #endif 
    { 
      //#pragma omp critical 
      //{
        #pragma omp barrier
        sys->nthreads = omp_get_num_threads();
        thid = omp_get_thread_num();
      //}
     //printf("Threads number %d reporting thread %d\n", sys->nthreads, thid);
    #endif             
    
    for (int i = 0; i < (sys->natoms ) - 1; i += step *  sys->nthreads)
    {
        #if defined(_MPI)
        #if defined(_OPENMP)
        ii =  i + sys->rank * sys->nthreads + thid;
        #else
        ii =  i + sys->rank;
        if (ii >=(sys->natoms-1)) break;
        #endif
        #else
        ii = i * sys->nthreads;
        #endif
        for (int j = ii + 1; j < (sys->natoms); ++j)
        {
            rx = pbc(sys->rx[ii] - sys->rx[j], 0.5 * sys->box);
            ry = pbc(sys->ry[ii] - sys->ry[j], 0.5 * sys->box);
            rz = pbc(sys->rz[ii] - sys->rz[j], 0.5 * sys->box);
            rsq = rx * rx + ry * ry + rz * rz;

            if (rsq < rcsq)
            {
                #if defined(_MORSE)
                double r = sqrt(rsq);
                double expoterm = exp(-alpha * (r - R));
                ffac = coeffs * expoterm * (expoterm - 1.0);
                #else
                double r6, rinv;
                rinv = 1.0 / rsq;
                r6 = rinv * rinv * rinv;
                ffac = (12.0 * c12 * r6 - 6.0 * c6) * r6 * rinv; // replace with morse
                #endif

                #if defined(_MPI)
                // Update forces using vectorized operations
                sys->cx[ii] += rx * ffac;
                sys->cy[ii] += ry * ffac;
                sys->cz[ii] += rz * ffac;

                // Newton's third law: opposite force on particle j
                sys->cx[j] -= rx * ffac;
                sys->cy[j] -= ry * ffac;
                sys->cz[j] -= rz * ffac;
                #if defined(_MORSE)
                epotsum += sys->epsilon * (1.0 - expoterm) * (1.0 - expoterm);
                #else
                epotsum += r6 * (c12 * r6 - c6);
                #endif
                #else
                // Update forces using vectorized operations
                sys->fx[ii] += rx * ffac;
                sys->fy[ii] += ry * ffac;
                sys->fz[ii] += rz * ffac;

                // Newton's third law: opposite force on particle j
                sys->fx[j] -= rx * ffac;
                sys->fy[j] -= ry * ffac;
                sys->fz[j] -= rz * ffac;
                #if defined(_MORSE)
                epot += sys->epsilon * (1.0 - expoterm) * (1.0 - expoterm);
                #else
                epot += r6 * (c12 * r6 - c6);
                #endif
                #endif
            }
        }
    }
    #if defined(_OPENMP)
    } // omp parallel
    #endif

    sys->epot = epot;
    #if defined(_MPI)
    MPI_Reduce(sys->cx, sys->fx, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->syscomm); 
    MPI_Reduce(sys->cy, sys->fy, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->syscomm); 
    MPI_Reduce(sys->cz, sys->fz, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->syscomm); 
    MPI_Reduce(&epotsum, &sys->epot, 1, MPI_DOUBLE, MPI_SUM, 0, sys->syscomm);
    #endif
}