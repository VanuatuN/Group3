#ifndef COMPUTE_FORCE_H
#define COMPUTE_FORCE_H
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "datastructure.h"
#include "utilities.h"

void velverlet(mdsys_t *sys);
void force(mdsys_t *sys);
#endif