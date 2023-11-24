#ifndef C_UTILITIES_H
#define C_UTILITIES_H
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <ctype.h>
#include "datastructure.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern int get_a_line(FILE *fp, char *buf, int BLEN);
extern double wallclock();
extern void azzero(double *d, const int n);
extern double pbc(double x, const double boxby2);
extern void ekin(mdsys_t *sys);
#ifdef __cplusplus
}
#endif

#endif