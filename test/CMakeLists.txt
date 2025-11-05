# Let us add unit testing for the example library
add_executable(test_example test_example.c)
# We need to link both with the library we are testing and the mtest framework.
target_link_libraries(test_example example_library mtest)

# Add test cases to CTest (our test framework has a nice macro for that).
discover_tests(test_example)
