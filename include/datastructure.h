#ifndef C_DATASTRUCTURE_H
#define C_DATASTRUCTURE_H
/* a few physical constants */
extern const double kboltz;// = 0.0019872067;     /* boltzman constant in kcal/mol/K */
extern const double mvsq2e; // = 2390.05736153349; /* m*v^2 in kcal/mol */

struct mdsys {
    int natoms,nfi,nsteps;
    double dt, mass, epsilon, sigma, box, rcut;
    double ekin, epot, temp;
    double *rx, *ry, *rz;
    double *vx, *vy, *vz;
    double *fx, *fy, *fz;
};
typedef struct mdsys mdsys_t;

extern void init_mdsys(mdsys_t *sys);
extern void update_mdsys(mdsys_t *sys);
extern void cleanup_mdsys(mdsys_t *sys);

#endif // DATASTRUCTURE_H
