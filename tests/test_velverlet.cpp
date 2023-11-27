#include <gtest/gtest.h>
#include "datastructure.h"
#include "utilities.h"
#include "compute_force.h"
#include "velverlet.h"
#include "datastructure.h"

class VelocityVerletTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        // Initialize your system for testing
        // For example, create an instance of mdsys_t and set initial values
        mdsys_t sys;
        init_mdsys(&sys);
        
        //sys = new mdsys_t;
        //initialize_system(sys);
    }

    virtual void TearDown() {
        // Clean up resources after testing
        delete sys;
    }

    mdsys_t* sys;
};

TEST_F(VelocityVerletTest, UpdateHalf1) {
    // Test update_half1 function

    // Set initial values for testing
    int i = 0; // or any other valid index
    sys->dt = 5.0; // set your desired time step
    
    std::string restfile = "../examples/argon_2.rest";
    read_restart(sys, restfile.c_str());

    // Save initial state for comparison
    double initial_rx = sys->rx[i];
    double initial_ry = sys->ry[i];
    double initial_rz = sys->rz[i];
    double initial_vx = sys->vx[i];
    double initial_vy = sys->vy[i];
    double initial_vz = sys->vz[i];

    // Call the function
    update_half1(sys, i);

    // Check if velocities and positions have been updated
    ASSERT_NE(initial_rx, sys->rx[i]);
    ASSERT_NE(initial_ry, sys->ry[i]);
    ASSERT_NE(initial_rz, sys->rz[i]);
    ASSERT_NE(initial_vx, sys->vx[i]);
    ASSERT_NE(initial_vy, sys->vy[i]);
    ASSERT_NE(initial_vz, sys->vz[i]);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
