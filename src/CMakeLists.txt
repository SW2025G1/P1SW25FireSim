set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})  # Place programs in root folder when compiled.

add_library(functions-c functions.c)
target_include_directories(functions-c PUBLIC .)
target_link_libraries(functions-c m)  # Link the math library, in case you need to use functions from <math.h>
add_library(simulation-c simulation.c)
target_include_directories(simulation-c PUBLIC .)
target_link_libraries(simulation-c PUBLIC m functions-c)

add_executable(main main.c)

target_link_libraries(main functions-c simulation-c m)
