#include <gtest/gtest.h>
#include "datastructure.h" // Include your data structure header
// Include your C functions here
extern "C" {
#include "utilities.h"      // Include your utility functions header
}
TEST(Utilities, PBC) {
    // Example test for the pbc function
    double result = pbc(5.0, 10.0);
    EXPECT_DOUBLE_EQ(result, 5.0); // You should modify this based on your expectations
}

TEST(Utilities, Ekin) {
    // Create a dummy mdsys_t structure
    mdsys_t sys;
    sys.natoms = 3; // Modify based on your system size
    sys.mass = 1.0; // Modify based on your particle mass

    // Initialize velocities (modify based on your system)
    sys.vx[0] = 1.0;
    sys.vy[0] = 2.0;
    sys.vz[0] = 3.0;

    sys.vx[1] = -1.0;
    sys.vy[1] = -2.0;
    sys.vz[1] = -3.0;

    sys.vx[2] = 0.5;
    sys.vy[2] = 1.5;
    sys.vz[2] = 2.5;

    // Call the ekin function
    ekin(&sys);

    // Add assertions to check the results
    EXPECT_DOUBLE_EQ(sys.ekin, 42.0); // You should modify this based on your expectations
    EXPECT_DOUBLE_EQ(sys.temp, 42.0 / (3.0 * sys.natoms - 3.0) / kboltz); // Modify based on your expectations
}

TEST(TestWallClock, one) {
    double t_test = -wallclock();
    // Use std::this_thread::sleep_for for sleeping
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    t_test += wallclock();
    ASSERT_TRUE(t_test >= 0.1);
    ASSERT_TRUE(t_test < 1.1);
}


TEST(TestAzzero, doubles) {
    double *buf = new double[10];
    for (int i = 0; i < 10; ++i) buf[i] = i + 1;
    ASSERT_DOUBLE_EQ(buf[1], 2.0);
    ASSERT_DOUBLE_EQ(buf[5], 6.0);
    ASSERT_DOUBLE_EQ(buf[9], 10.0);

    azzero(buf, 10);
    ASSERT_DOUBLE_EQ(buf[1], 0.0);
    ASSERT_DOUBLE_EQ(buf[5], 0.0);
    ASSERT_DOUBLE_EQ(buf[9], 0.0);

    delete[] buf; // Don't forget to free the allocated memory
}

TEST(Utilities, GetALine) {
    // Create a temporary file for testing
    FILE *tmpFile = tmpfile();

    // Write a line to the file
    const char *testLine = "   This is a test line with whitespace and comments # Comment";
    fputs(testLine, tmpFile);
    rewind(tmpFile);

    // Call the get_a_line function
    const int BLEN = 100; // Adjust based on your buffer size
    char buf[BLEN];
    int result = get_a_line(tmpFile, buf, BLEN);

    // Add assertions to check the results
    ASSERT_EQ(result, 0);
    ASSERT_STREQ(buf, "This is a test line with whitespace and comments");

    // Close the temporary file
    fclose(tmpFile);
}

// Add more tests for other utility functions as needed

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
