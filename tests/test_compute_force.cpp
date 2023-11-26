// tests/test_compute_force.cpp
#include "gtest/gtest.h"
#include "utilities.h"
#include "compute_force.h"

TEST(ComputeForce, TwoParticleInsideCutoff) {
    mdsys_t sys;
    init_mdsys(&sys);
    sys.natoms = 2;
    sys.mass = 39.948;
    sys.epsilon = 0.2379;
    sys.sigma = 3.405;
    sys.rcut = 8.5; // Set your cutoff distance
    sys.box = 17.1580;

    sys.epot=0.0;
    azzero(sys.fx,sys.natoms);
    azzero(sys.fy,sys.natoms);
    azzero(sys.fz,sys.natoms);

    // Set positions of particles inside cutoff
    sys.rx[0] = 0.0;
    sys.ry[0] = 0.0;
    sys.rz[0] = 0.0;

    sys.rx[1] = 3.0;
    sys.ry[1] = 4.0;
    sys.rz[1] = 0.0;

    force(&sys);

    // Add assertions to check the forces
    EXPECT_DOUBLE_EQ(sys.fx[0], 0.054706442224795614);
    EXPECT_DOUBLE_EQ(sys.fy[0], 0.072941922966394165);
    EXPECT_DOUBLE_EQ(sys.fz[0], 0.0);
}

TEST(ComputeForce, TwoParticleInsideCutoffNearCuttoff) {
    mdsys_t sys;
    init_mdsys(&sys);
    sys.natoms = 2;
    sys.mass = 39.948;
    sys.epsilon = 0.2379;
    sys.sigma = 3.405;
    sys.rcut = 8.5; // Set your cutoff distance
    sys.box = 17.1580;

    sys.epot=0.0;
    azzero(sys.fx,sys.natoms);
    azzero(sys.fy,sys.natoms);
    azzero(sys.fz,sys.natoms);

    // Set positions of particles inside cutoff
    sys.rx[0] = 0.0;
    sys.ry[0] = 0.0;
    sys.rz[0] = 0.0;

    sys.rx[1] = 0.0;
    sys.ry[1] = 8.45;
    sys.rz[1] = 0.0;

    force(&sys);

    // Add assertions to check the forces
    EXPECT_DOUBLE_EQ(sys.fx[0], 0.0);
    EXPECT_DOUBLE_EQ(sys.fy[0], 0.0028679847024252691);
    EXPECT_DOUBLE_EQ(sys.fz[0], 0.0);
}

TEST(ComputeForce, TwoParticleOutsideCutoff) {
    mdsys_t sys;
    init_mdsys(&sys);
    sys.natoms = 2;
    sys.mass = 39.948;
    sys.epsilon = 0.2379;
    sys.sigma = 3.405;
    sys.rcut = 8.5; // Set your cutoff distance
    sys.box = 17.1580;

    sys.epot = 0.0;
    azzero(sys.fx, sys.natoms);
    azzero(sys.fy, sys.natoms);
    azzero(sys.fz, sys.natoms);

    // Set positions of particles outside cutoff
    sys.rx[0] = 0.0;
    sys.ry[0] = 0.0;
    sys.rz[0] = 0.0;

    sys.rx[1] = 10.0;
    sys.ry[1] = 10.0;
    sys.rz[1] = 0.0;

    force(&sys);

    // Add assertions to check the forces
    EXPECT_DOUBLE_EQ(sys.fx[0], 0.0);
    EXPECT_DOUBLE_EQ(sys.fy[0], 0.0);
    EXPECT_DOUBLE_EQ(sys.fz[0], 0.0);
}

TEST(ComputeForce, TwoParticleInsideCutoffPBC) {
    mdsys_t sys;
    init_mdsys(&sys);
    sys.natoms = 2;
    sys.mass = 39.948;
    sys.epsilon = 0.2379;
    sys.sigma = 3.405;
    sys.rcut = 8.5; // Set your cutoff distance
    sys.box = 17.1580;

    sys.epot=0.0;
    azzero(sys.fx,sys.natoms);
    azzero(sys.fy,sys.natoms);
    azzero(sys.fz,sys.natoms);

    // Set positions of particles inside cutoff
    sys.rx[0] = 0.0;
    sys.ry[0] = 0.0;
    sys.rz[0] = 0.0;

    sys.rx[1] = 17.0;
    sys.ry[1] = 0.0;
    sys.rz[1] = 0.0;

    force(&sys);

    // Add assertions to check the forces
    EXPECT_DOUBLE_EQ(sys.fx[0], 7.2526871094147392e+17);
    EXPECT_DOUBLE_EQ(sys.fy[0], 0.0);
    EXPECT_DOUBLE_EQ(sys.fz[0], 0.0);
}

TEST(ComputeForce, TwoParticleInsideCutoffPBC1) {
    mdsys_t sys;
    init_mdsys(&sys);
    sys.natoms = 2;
    sys.mass = 39.948;
    sys.epsilon = 0.2379;
    sys.sigma = 3.405;
    sys.rcut = 8.5; // Set your cutoff distance
    sys.box = 17.1580;

    sys.epot=0.0;
    azzero(sys.fx,sys.natoms);
    azzero(sys.fy,sys.natoms);
    azzero(sys.fz,sys.natoms);

    // Set positions of particles inside cutoff
    sys.rx[0] = 0.0;
    sys.ry[0] = 0.0;
    sys.rz[0] = 0.0;

    sys.rx[1] = 0.0;
    sys.ry[1] = 0.0;
    sys.rz[1] = 11.0;

    force(&sys);

    // Add assertions to check the forces
    EXPECT_DOUBLE_EQ(sys.fx[0], 0.0);
    EXPECT_DOUBLE_EQ(sys.fy[0], 0.0);
    EXPECT_DOUBLE_EQ(sys.fz[0], -0.024984372427535984);
}