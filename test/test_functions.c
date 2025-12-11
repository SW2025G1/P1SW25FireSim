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



//========tests for get_file_path_from_user funktionen======
 TEST_CASE(get_file_path_from_user_test, {
     // Arrange
     FILE* expected = "maps/TL1_36x36.txt";
     // Act
     FILE* result = get_file_path_from_user();
     // Assert
     assert(result == expected);
 })


// Register your test cases to this macro:
MAIN_RUN_TESTS(get_file_path_from_user_test)
