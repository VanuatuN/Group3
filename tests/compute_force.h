// #ifndef C_COMPUTE_FORCE_H
// #define C_COMPUTE_FORCE_H
// #include <stdio.h>
// #include <string.h>
// #include <math.h>
// #include "datastructure.h"
// #include "utilities.h"

// extern "C" {
// void force(mdsys_t *sys);
// }

// #endif


// // compute_force.h
#ifndef C_COMPUTE_FORCE_H
#define C_COMPUTE_FORCE_H
#include "datastructure.h"

/* compute forces */
#ifdef __cplusplus
extern "C" {
void force(mdsys_t *sys);
} 
#else 
void force(mdsys_t *sys);
#endif


#endif /* COMPUTE_FORCE_H */
