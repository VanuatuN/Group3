#include <string>
#include "gtest/gtest.h"
#include "datastructure.h"

TEST(ReadRestart, TwoAtoms)
{
    mdsys_t sys;
    init_mdsys(&sys);
    sys.natoms = 2;

    std::string restfile = "../examples/argon_2.rest";

    read_restart(&sys, restfile.c_str());
    
    EXPECT_DOUBLE_EQ(sys.rx[0], 6.67103294321331);
    EXPECT_DOUBLE_EQ(sys.ry[0], -10.6146871435653);
    EXPECT_DOUBLE_EQ(sys.rz[0], 12.6336939877734);

    EXPECT_DOUBLE_EQ(sys.vx[0], -1.5643224621482283e-03);
    EXPECT_DOUBLE_EQ(sys.vy[0], 4.8497508563925346e-04);
    EXPECT_DOUBLE_EQ(sys.vz[0], -4.3352481732883966e-04);
}
