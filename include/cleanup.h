#ifndef CLEANUP_H
#define CLEANUP_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "datastructure.h"
#include "utilities.h"

void cleanup(FILE *erg, FILE *traj, mdsys_t *sys, double t_start);

#endif
