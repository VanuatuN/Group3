#ifndef C_UTILITIES_H
#define C_UTILITIES_H

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <ctype.h>
#include "datastructure.h"

extern int get_a_line(FILE *fp, char *buf);
extern double wallclock();
extern void azzero(double *d, const int n);
extern double pbc(double x, const double boxby2);
extern void ekin(mdsys_t *sys);
#endif