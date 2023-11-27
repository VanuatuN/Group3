#ifndef C_DATASTRUCTURE_H
#define C_DATASTRUCTURE_H

#ifdef __cplusplus
extern "C" {
#endif

/* a few physical constants */
extern const double kboltz; // = 0.0019872067;     /* boltzmann constant in kcal/mol/K */
extern const double mvsq2e; // = 2390.05736153349; /* m*v^2 in kcal/mol */

struct mdsys {
    int natoms, nfi, nsteps;
    double dt, mass, epsilon, sigma, box, rcut;
    double ekin, epot, temp;
    double *rx, *ry, *rz;
    double *vx, *vy, *vz;
    double *fx, *fy, *fz;
};
typedef struct mdsys mdsys_t;
void init_mdsys(mdsys_t *sys);
void cleanup_mdsys(mdsys_t *sys);
void read_restart(mdsys_t *sys, const char *restfile);

#ifdef __cplusplus
}
#endif

#endif
