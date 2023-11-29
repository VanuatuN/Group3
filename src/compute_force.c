#include <stdio.h>
#include <string.h>
#include <math.h>
#include "datastructure.h"
#include "utilities.h"

#if defined(USE_MPI)
#include "mpi.h"
#endif

#if defined(USE_OPENMP)
#include "omp.h"
 printf("Thread %d reporting\n", thid,nthds);
#endif

/* compute forces */
void force(mdsys_t *sys)
{
    double rsq_inv,r6;
    double c12 = 4.0 * sys->epsilon * pow(sys->sigma, 12.0);
    double c6 = 4.0 * sys->epsilon * pow(sys->sigma, 6.0);
    double rcsq = sys->rcut * sys->rcut;
    double ffac, rx, ry, rz, rsq;
    double * fx, * fy, * fz; // local pointers for openmp
    double epot=0.0; // needed for reduction with openmp
    int niters = (sys->natoms) * (sys->natoms - 1) / 2; // to linearize (and balance) the loop with openmp / mpi
    int step = 1;
    //sys->epot = 0.0;
    int ii,i,j,n,offset;
    
    int npes, rank, local_niter, lower_bound, upper_bound;

    #if defined(_MPI)

    double epotsum = 0.0;
    step = sys->npes;

    local_niter = niters/npes;
    lower_bound = local_niter * rank;
    upper_bound = (rank + 1) * local_niter;
    
    // azzero(sys->cx, sys->natoms);
    // azzero(sys->cy, sys->natoms);
    // azzero(sys->cz, sys->natoms);

    MPI_Bcast(sys->rx, sys->natoms, MPI_DOUBLE, 0, sys->syscomm);
    MPI_Bcast(sys->ry, sys->natoms, MPI_DOUBLE, 0, sys->syscomm);
    MPI_Bcast(sys->rz, sys->natoms, MPI_DOUBLE, 0, sys->syscomm);
    
    #else
    npes = 1;
    rank = 0;
    local_niter = niters;
    lower_bound = 0;
    upper_bound = niters;
    #endif // of the *USE_MPI* //

      double boxby2 = 0.5*sys->box;// pre-calculate
      double rcutsq = sys->rcut*sys->rcut;// pre-calculate, take square
      //double c6 = sys->epsilon*pow(sys->sigma,6);
      //double c12 = sys->epsilon*pow(sys->sigma,12);

  #if defined(USE_OPENMP)
  #pragma omp parallel private(fx,fy,fz,i,j,rx,ry,rz,rsq,rsq_inv,r6,ffac,offset) reduction(+:epot)
  #endif
    {

        // who is who
        int thid; // thread id
        int nthds; // number of threads

    #if defined(USE_OPENMP)
        thid = omp_get_thread_num();
        nthds = omp_get_num_threads();
     #else
        thid = 0;
        nthds = 1;
     #endif   
     
      // assign slices of force array to threads
        fx = sys->fx + (thid * sys->natoms);
        fy = sys->fy + (thid * sys->natoms);
        fz = sys->fz + (thid * sys->natoms);
        
        // zero energy and forces
        azzero(fx,sys->natoms);
        azzero(fy,sys->natoms);
        azzero(fz,sys->natoms);
        
        // main loop
     #if defined(USE_OPENMP)
     #ifndef CHUNKSIZE
     #define CHUNKSIZE 1
     #endif
     #pragma omp for schedule(guided,CHUNKSIZE)
     #endif
    
         for(n=lower_bound; n<upper_bound; ++n) {
            i = sys->natoms - 2 - (int)(sqrt(-8*n + 4*sys->natoms*(sys->natoms-1)-7)/2.0 - 0.5);
            j = n + i + 1 - sys->natoms*(sys->natoms-1)/2 + (sys->natoms-i)*((sys->natoms-i)-1)/2;
          
           // get distance between particle i and j
            rx=pbc(sys->rx[i] - sys->rx[j], boxby2);
            rsq = rx*rx;
            if(rsq>rcutsq) continue;
            ry=pbc(sys->ry[i] - sys->ry[j], boxby2);
            rsq += ry*ry;
            if(rsq>rcutsq) continue;
            rz=pbc(sys->rz[i] - sys->rz[j], boxby2);
            rsq += rz*rz;
            if(rsq>rcutsq) continue;

            rsq_inv = 1.0/rsq;
            r6 = rsq_inv*rsq_inv*rsq_inv;
            ffac = (48*c12*r6-24*c6)*r6*rsq_inv;
            epot += 4*r6*(c12*r6-c6);
            fx[i] += rx*ffac;
            fx[j] -= rx*ffac;
            fy[i] += ry*ffac;
            fy[j] -= ry*ffac;
            fz[i] += rz*ffac;
            fz[j] -= rz*ffac;
        }

        // after a work sharing construct an omp barrier is implied
#if defined(USE_OPENMP)
#pragma omp for
#endif
            

        for (i=0; i<sys->natoms; ++i) {
            for (offset=sys->natoms; offset<nthds*sys->natoms; offset+=sys->natoms) {
                sys->fx[i] += sys->fx[offset + i];
                sys->fy[i] += sys->fy[offset + i];
                sys->fz[i] += sys->fz[offset + i];
            }
        }
        
    }
    
    sys->epot = epot;
    
    #if defined(_MPI)
    MPI_Reduce(sys->cx, sys->fx, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->syscomm); 
    MPI_Reduce(sys->cy, sys->fy, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->syscomm); 
    MPI_Reduce(sys->cz, sys->fz, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->syscomm); 
    MPI_Reduce(&epotsum, &sys->epot, 1, MPI_DOUBLE, MPI_SUM, 0, sys->syscomm);
    #endif
}