# Let us add unit testing for the example library
add_executable(test_functions test_functions.c ../src/input-output.c)

# adding simulation testing
add_executable(test_simulations
        test_simulations.c
        ../src/simulation.c
        ../src/input-output.c
)

# We need to link both with the library we are testing and the mtest framework.
target_link_libraries(test_functions input-output-c mtest)

#same but for simulations
target_link_libraries(test_simulations input-output-c simulation-c mtest)

# Add test cases to CTest (our test framework has a nice macro for that).
discover_tests(test_functions)

#again for simulations
discover_tests(test_simulations)
