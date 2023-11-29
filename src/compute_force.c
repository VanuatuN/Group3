#include <stdio.h>
#include <string.h>
#include <math.h>
#include "datastructure.h"
#include "utilities.h"
#ifndef REFACTOR
/* compute forces */
void force(mdsys_t *sys)
{
    double r,ffac;
    double rx,ry,rz;
    int i,j;

    /* zero energy and forces */
    sys->epot=0.0;
    azzero(sys->fx,sys->natoms);
    azzero(sys->fy,sys->natoms);
    azzero(sys->fz,sys->natoms);

    for(i=0; i < (sys->natoms); ++i) {
        for(j= 0; j < (sys->natoms); ++j) {

            /* particles have no interactions with themselves */
            if (i==j) continue;

            /* get distance between particle i and j */
            rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
            ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
            rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
            r = sqrt(rx*rx + ry*ry + rz*rz);

            /* compute force and energy if within cutoff */
            if (r < sys->rcut) {
                ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r,12.0)/r
                                         +6*pow(sys->sigma/r,6.0)/r);

                sys->epot += 0.5*4.0*sys->epsilon*(pow(sys->sigma/r,12.0)
                                               -pow(sys->sigma/r,6.0));

                sys->fx[i] += rx/r*ffac;
                sys->fy[i] += ry/r*ffac;
                sys->fz[i] += rz/r*ffac;
            }
        }
    }
}

#else


/* Optimized force computation */
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

#endif
