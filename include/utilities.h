#ifndef C_UTILITIES_H
#define C_UTILITIES_H
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <ctype.h>
#include "datastructure.h"

extern const double kboltz;   /* boltzman constant in kcal/mol/K */
extern const double mvsq2e; /* m*v^2 in kcal/mol */

#ifdef __cplusplus
extern "C" {
#endif
extern int get_a_line(FILE *fp, char *buf, int BLEN);
extern double wallclock();
extern void azzero(double *d, const int n);
extern double pbc(double x, const double boxby2);
extern void ekin(mdsys_t *sys);
#if defined(_MPI)
extern void mpi_init();
extern void mpi_finalize();
extern void mpi_get_rank(mdsys_t *sys);
extern void mpi_get_size(mdsys_t *sys);
#endif

#ifdef __cplusplus
}
#endif

#endif