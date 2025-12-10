#include "mtest.h"
#include "../src/simulation.h"

// Test the open file function
TEST_CASE(test, {
    // Arrange
    // Act
    // Assert
})


TEST_CASE(example_fail, {
    CHECK_FAIL("This test fails, so we can see how a failing test case looks like.");

})

// Register your test cases to this macro:
MAIN_RUN_TESTS(test, example_fail)
