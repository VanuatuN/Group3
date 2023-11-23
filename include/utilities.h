#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <ctype.h>
#include "datastructure.h"

static int get_a_line(FILE *fp, char *buf);
static double wallclock();
static void azzero(double *d, const int n);
static double pbc(double x, const double boxby2);
static void ekin(mdsys_t *sys);