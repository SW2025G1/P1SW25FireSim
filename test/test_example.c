#include "mtest.h"
#include "example_library.h"

// Write test cases like this:
TEST_CASE(example_pass, {
    int expected = 2, number = 1;      // Arrange
    int result = add(number, number);  // Act
    CHECK_EQ_INT(result, expected);    // Assert
})

TEST_CASE(example_fail, {
    CHECK_FAIL("This test fails, so we can see how a failing test case looks like.");
})

// Register your test cases to this macro:
MAIN_RUN_TESTS(example_pass, example_fail)
