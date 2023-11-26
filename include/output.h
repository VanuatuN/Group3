#ifndef C_OUTPUT_H
#define C_OUTPUT_H
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include "datastructure.h"
#include "utilities.h"

#ifdef __cplusplus
extern "C" {
#endif

void output(mdsys_t *sys, FILE *erg, FILE *traj);

#ifdef __cplusplus
}
#endif

#endif