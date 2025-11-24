#include "mtest.h"
#include "functions.h"
#include "simulation.h"
// Test the open file function
TEST_CASE(open_data_test, {
    char file_name[] = "src/Data.txt";
    int expected = 0; // Arrange
    FILE* fptr = open_data(file_name); // Act
    CHECK_TRUE(fptr != NULL); // Assert
})


TEST_CASE(example_fail, {
    CHECK_FAIL("This test fails, so we can see how a failing test case looks like.");
})

// Register your test cases to this macro:
MAIN_RUN_TESTS(open_data_test)
