#include <gtest/gtest.h>
#include <thread>   // Include for std::this_thread::sleep_for
#include <chrono>   // Include for std::chrono::milliseconds
#include "datastructure.h"
#include "utilities.h"      // Include your utility functions header

TEST(Utilities, PBC) {
    // Example test for the pbc function
    double result = pbc(5.0, 10.0);
    EXPECT_DOUBLE_EQ(result, 5.0); // You should modify this based on your expectations
}

TEST(Utilities, Ekin) {
    // Create a dummy mdsys_t structure
    // /* a few physical constants */
    extern const double kboltz;     /* boltzman constant in kcal/mol/K */
    extern const double mvsq2e; /* m*v^2 in kcal/mol */
    mdsys_t sys;
    init_mdsys(&sys);
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
    // Print intermediate values for debugging
    std::cout << "sys.ekin: " << sys.ekin << std::endl;
    std::cout << "sys.temp: " << sys.temp << std::endl;

    // Add assertions to check the results
    // Assuming epsilon is a small positive value, e.g., 1e-6
    const double epsilon = 2e-5;

    // Add assertions to check the results with a tolerance
    EXPECT_NEAR(sys.ekin, 43917.3040, epsilon);
    EXPECT_NEAR(sys.temp, 2.0 * sys.ekin / (3.0 * sys.natoms - 3.0) / kboltz, epsilon);

    cleanup_mdsys(&sys);
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

    delete[] buf; // free the allocated memory
}


TEST(Utilities, GetALine) {
    // Test the get_a_line function

    // Example content for a temporary file
    const char* fileContent =
        "This is line 1\n"
        "   # Comment line\n"
        "  Line 3 with trailing spaces    \n"
        "  \n"; // Empty line

    // Create a temporary file for testing
    FILE* testFile = fopen("test_get_a_line.txt", "w+");
    if (testFile != nullptr) {
        fputs(fileContent, testFile);
        fclose(testFile);

        // Open the temporary file for reading
        FILE* readFile = fopen("test_get_a_line.txt", "r");

        // Buffer for storing lines
        const int BLEN = 256;
        char line[BLEN];

        // Test get_a_line function
        ASSERT_EQ(get_a_line(readFile, line, BLEN), 0);
        EXPECT_STREQ(line, "This is line 1");

        ASSERT_EQ(get_a_line(readFile, line, BLEN), 0);
        EXPECT_STREQ(line, "");

        ASSERT_EQ(get_a_line(readFile, line, BLEN), 0);
        EXPECT_STREQ(line, "  Line 3 with trailing spaces");

        ASSERT_EQ(get_a_line(readFile, line, BLEN), 0);
        EXPECT_STREQ(line, "");

        fclose(readFile);
        remove("test_get_a_line.txt");
    } else {
        FAIL() << "Error creating temporary file.";
    }
}

