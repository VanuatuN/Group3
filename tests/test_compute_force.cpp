// tests/test_compute_force.cpp
#include "gtest/gtest.h"
#include "compute_force.h"

TEST(ComputeForce, TwoParticleInsideCutoff) {
    mdsys_t sys;
    sys.natoms = 2;
    sys.rcut = 5.0; // Set your cutoff distance

    // Set positions of particles inside cutoff
    sys.rx[0] = 0.0;
    sys.ry[0] = 0.0;
    sys.rz[0] = 0.0;

    sys.rx[1] = 3.0;
    sys.ry[1] = 4.0;
    sys.rz[1] = 0.0;

    force(&sys);

    // Add assertions to check the forces
    EXPECT_DOUBLE_EQ(sys.fx[0], 0.0);
    EXPECT_DOUBLE_EQ(sys.fy[0], 0.0);
    EXPECT_DOUBLE_EQ(sys.fz[0], 0.0);

}


