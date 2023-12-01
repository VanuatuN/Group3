#include <stdio.h>
#include <string.h>
#include "datastructure.h"
#include "utilities.h"

int input(FILE *fp, char *line, char *restfile, char *trajfile, char *ergfile, mdsys_t *sys, int *nprint, int BLEN){
    /* read input file */
    if(get_a_line(stdin,line, BLEN)) return 1;
    sys->natoms=atoi(line);
    if(get_a_line(stdin,line, BLEN)) return 1;
    sys->mass=atof(line);
    if(get_a_line(stdin,line, BLEN)) return 1;
    sys->epsilon=atof(line);
    if(get_a_line(stdin,line, BLEN)) return 1;
    sys->sigma=atof(line);
    if(get_a_line(stdin,line, BLEN)) return 1;
    sys->rcut=atof(line);
    if(get_a_line(stdin,line, BLEN)) return 1;
    sys->box=atof(line);
    if(get_a_line(stdin,restfile, BLEN)) return 1;
    if(get_a_line(stdin,trajfile, BLEN)) return 1;
    if(get_a_line(stdin,ergfile, BLEN)) return 1;
    if(get_a_line(stdin,line, BLEN)) return 1;
    sys->nsteps=atoi(line);
    if(get_a_line(stdin,line, BLEN)) return 1;
    sys->dt=atof(line);
    if(get_a_line(stdin,line, BLEN)) return 1;
    *nprint=atoi(line);
    return 0;
}
