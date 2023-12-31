#ifndef VELVERLET_H
#define VELVERLET_H
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "datastructure.h"
#include "utilities.h"
#include "compute_force.h"

#ifdef __cplusplus
extern "C" {
#endif

void update_half1(mdsys_t *sys, int i);
void update_half2(mdsys_t *sys, int i);
void velverlet(mdsys_t *sys);

#ifdef __cplusplus
}
#endif

#endif