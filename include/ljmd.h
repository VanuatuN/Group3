#ifndef LJMD_H
#define LJMD_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _mdsys {
    int natoms;
    double mass;
    double *rx;
    double *vx;
    double *fx;
};

typedef struct _mdsys mdsys_t;

// Function declarations
int get_a_line(FILE *fp, char *buf, int BLEN);
double wallclock();
void azzero(double *d, const int n);
double pbc(double x, const double boxby2);
void ekin(mdsys_t *sys);

#ifdef __cplusplus
}
#endif

#endif
