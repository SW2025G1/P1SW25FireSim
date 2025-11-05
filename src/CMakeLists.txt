set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})  # Place programs in root folder when compiled.

add_library(example_library example_library.c)
target_include_directories(example_library PUBLIC .)
target_link_libraries(example_library m)  # Link the math library, in case you need to use functions from <math.h>

add_executable(main main.c)
target_link_libraries(main example_library)
