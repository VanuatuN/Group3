#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include<stdio.h>
#include<sys/time.h>
#include<math.h>
#include<string.h>
struct _mdsys {
    int natoms,nfi,nsteps;
    double dt, mass, epsilon, sigma, box, rcut;
    double ekin, epot, temp;
    double *rx, *ry, *rz;
    double *vx, *vy, *vz;
    double *fx, *fy, *fz;
};
typedef struct _mdsys mdsys_t;

static void output(mdsys_t *sys, FILE *erg, FILE *traj);
static void velverlet(mdsys_t *sys);
static void force(mdsys_t *sys);
static void ekin(mdsys_t *sys);
static double pbc(double x, const double boxby2);
static void azzero(double *d, const int n);
static double wallclock();
static int get_a_line(FILE *fp, char *buf);
static int input(FILE *fp,char *buf, char *line, char *restfile, char *trajfile, char *ergfile);
