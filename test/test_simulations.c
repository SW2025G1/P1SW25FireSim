#include "mtest.h"
#include "../src/simulation.h"

// Mock output_and_open_html to be able to run test
void output_and_open_html(int simulation_run_count, map_t* map,
                          double time_spent, int input_time, int all_time) {
    // Empty
}

// Test the timekeeper
TEST_CASE(test_timekeeper_success, {
    // Arrange
    int input_time = 3;
    int all_time = 0;
    int expected = 3; //expected all_time value
    // Act
    update_timekeeper(input_time, &all_time);
    // Assert
    assert(all_time == expected);
})

//This test should fail
TEST_CASE(test_timekeeper_fail, {
    // Arrange
    int input_time = 9;
    int all_time = 0;
    int expected = 9;
    // Act
    update_timekeeper(input_time, &all_time);
    // Assert
    assert(all_time != expected); //fails since we expected that the value is not 9 but it is
    CHECK_FAIL("This test fails, so we can see how a failing test case looks like.");
})

TEST_CASE(test_get_neighbor_index_east, {
    //Arrange
    map_t map;
    map.size_of_map = 3; //3x3
    int expected = 5; //1 * 3 + 2 = 5
    //Act
    int result = get_neighbor_index(&map, 1, 1, East);
    //Assert
    assert(result == expected);
})



// Register your test cases to this macro:
MAIN_RUN_TESTS(test_timekeeper_success, test_timekeeper_fail, test_get_neighbor_index_east)
