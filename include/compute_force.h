// compute_force.h
#ifndef C_COMPUTE_FORCE_H
#define C_COMPUTE_FORCE_H
#include "datastructure.h"
#if defined(_MPI)
#include "mpi.h"
#endif

#if defined(_OPENMP)
#include "omp.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void force(mdsys_t *sys);

#ifdef __cplusplus
}
#endif

#endif /* COMPUTE_FORCE_H */
