#include "mtest.h"
#include "../src/simulation.h"

// Test the open file function

//=======tests for data funktion===============
/*TEST_CASE(open_data_test, {
    char file_name[] = "src/Data.txt";
    int expected = 0; // Arrange
    FILE* fptr = open_data(file_name); // Act
    CHECK_TRUE(fptr != NULL); // Assert
})*/ //open_data funktionen findes ikke længere. Hedder nu get_data_from_file.


TEST_CASE(example_fail, {
    CHECK_FAIL("This test fails, so we can see how a failing test case looks like.");
})


//========tests for get_file_path_from_user funktionen======
TEST_CASE(get_file_path_from_user_test, {
    // Arrange
    // Act
    // Assert
})

//======test get_size_of_map funktionen ===============
TEST_CASE(get_size_of_map_test, {

})

//=======test initialize_array funktionen ============
TEST_CASE(initialize_array_test, {
    // Arrange

    // Act
    // Assert
})



// Register your test cases to this macro:
MAIN_RUN_TESTS(get_file_path_from_user_test, get_size_of_map_test, initialize_array_test, example_fail)
