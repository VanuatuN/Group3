#include <gtest/gtest.h>
#include "datastructure.h"
#include "utilities.h"
#include "compute_force.h"
#include "velverlet.h"
#include "datastructure.h"
#include <stdio.h>
#include <string.h>


TEST(VelocityVerletTest, UpdateHalf1) {
    // Test update_half1 function
    mdsys_t sys;
    init_mdsys(&sys);

    sys.natoms = 2;
    sys.mass = 39.948;
    sys.epsilon = 0.2379;
    sys.sigma = 3.405;
    sys.rcut = 8.5; // Set your cutoff distance
    sys.box = 17.1580;

    sys.dt = 5.0;
    int i = 0; // or any other valid index

    // Save initial state for comparison

    sys.rx[i] = 0.0;
    sys.ry[i] = 0.0;
    sys.rz[i] = 0.0;

    sys.vx[i] = 0.1;
    sys.vy[i] = 0.1;
    sys.vz[i] = 0.1;

    sys.fx[i] = 0.1;
    sys.fy[i] = 0.1;
    sys.fz[i] = 0.1;

    double initial_rx = sys.rx[i];
    double initial_ry = sys.ry[i];
    double initial_rz = sys.rz[i];
    double initial_vx = sys.vx[i];
    double initial_vy = sys.vy[i];
    double initial_vz = sys.vz[i];

    // Call the function
    update_half1(&sys, i);

    // Check if velocities and positions have been updated
    ASSERT_NE(initial_rx, sys.rx[i]);
    ASSERT_NE(initial_ry, sys.ry[i]);
    ASSERT_NE(initial_rz, sys.rz[i]);
    ASSERT_NE(initial_vx, sys.vx[i]);
    ASSERT_NE(initial_vy, sys.vy[i]);
    ASSERT_NE(initial_vz, sys.vz[i]);
}

