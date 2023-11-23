#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <ctype.h>
#include "datastructure.h"

int get_a_line(FILE *fp, char *buf, int BLEN);
double wallclock();
void azzero(double *d, const int n);
double pbc(double x, const double boxby2);
void ekin(mdsys_t *sys);